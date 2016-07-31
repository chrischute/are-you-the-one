/**
 * ayto.cpp
 * Christopher Chute
 * 
 * Guess generator for MTV's Are You The One?
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include "ayto.h"

#define DIGITS ("0123456789") // Available digits for a permutation.
#define GUESS  ("4579108623") // The fixed guess made in the second turn.
#define NUM_THREADS (10)      // Number of threads to use during minimax.
#define PERM_LENGTH (10)      // Number of elements in a permutation.
#define POOL_FILENAME ("scripts/pool.txt") // Location of fixed guess pool.
#define START_FULL_MM (314)   // Start w/ full minimax when <= this # left.
#define START_PART_MM (10000) // Start partial minimax when <= this # left.

using std::abs;
using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::pair;
using std::rand;
using std::setw;
using std::srand;
using std::string;
using std::stringstream;
using std::thread;
using std::vector;

int main(int argc, char** argv) {
    AytoSettings* settings = new AytoSettings();

    if (settings->initializeFromArgs(argc, argv)) {
        if (settings->_isAllPermutationsMode) {
            cout << "Running on all possible answers." << endl;
            PerfectMatching answer = DIGITS;
            do {
                runAreYouTheOne(answer, settings);
            } while (next_permutation(answer.begin(), answer.end()));
        } else if (settings->_isReadFromFileMode) {
            cout << "Reading answers from " << settings->_fileToRead << "." << endl;
            PmSet *answers = new PmSet();
            answers->populateFromFile(settings->_fileToRead);
            for (PmSet::const_iterator answer = answers->begin();
                 answer != answers->end();
                 ++answer) {
                runAreYouTheOne(*answer, settings);
            }
            delete answers;
        } else if (settings->_isInteractiveMode) {
            cout << "Interactive mode. I think I'm going to win." << endl;
            runAreYouTheOne("", settings);
        } else {
            cout << "Running on a random answer." << endl;
            std::srand(unsigned(std::time(nullptr)));
            Pm answer = getRandomPerfectMatching();
            runAreYouTheOne(answer, settings);
        }
    } else {
        cout << "usage: ./ayto" << endl;
        cout << "\t[-a | -all]     * Run on all permutations" << endl;
        cout << "\t[-f <filename>] * Run on permutations in file" << endl;
        cout << "\t[-i]            * Interactive, user gives feedback" << endl;
        cout << "\t[-v]            * Verbose mode, more printing" << endl;
    }

    delete settings;

    return EXIT_SUCCESS;
}

bool AreYouTheOneSettings::initializeFromArgs(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "-a", 3) == 0) {
            this->_isAllPermutationsMode = true;
        } else if (strncmp(argv[i], "-f", 3) == 0) {
            this->_isReadFromFileMode = true;
            if (++i == argc) {
                return false;
            }
            this->_fileToRead = argv[i];
        } else if (strncmp(argv[i], "-i", 3) == 0) {
            this->_isInteractiveMode = true;
        } else if (strncmp(argv[i], "-v", 3) == 0) {
            this->_isVerboseMode = true;
        } else if (strncmp(argv[i], "-n", 3) == 0) {
            this->_isPrintNumbersMode = true;
        } else {
            return false;
        }
    }

    // Dictionaries for printing to the console. Males map char -> name
    // because the PerfectMatching string is a permutation of their names.
    if (!this->_isPrintNumbersMode) {
        this->_femaleNames = new map<int, string>{
                {0, "Alyssa"},
                {1, "Camille"},
                {2, "Emma"},
                {3, "Francesca"},
                {4, "Julia"},
                {5, "Kaylen"},
                {6, "Mikala"},
                {7, "Nicole"},
                {8, "Tori"},
                {9, "Victoria"}
        };

        this->_maleNames = new map<char, string>{
                {'0', "Asaf"},
                {'1', "Cam"},
                {'2', "Cameron"},
                {'3', "Giovanni"},
                {'4', "John"},
                {'5', "Morgan"},
                {'6', "Prosper"},
                {'7', "Sam"},
                {'8', "Stephen"},
                {'9', "Tyler"}
        };
    }

    return true;
}

Pm getNextPerfectMatchingGuess(PmSet* possibleAnswers, PmSet* guessesAlreadyMade)
{
    if (possibleAnswers->size() == 1) {
        return possibleAnswers->get(0);
    }

    switch (guessesAlreadyMade->size()) {
        case 0:
            return DIGITS;
        case 1:
            return GUESS;
        default:
            return getNextGuessUsingMinimax(possibleAnswers, guessesAlreadyMade);
    }
}

Tb getNextTruthBoothGuess(PmSet* possibleAnswers, TbSet* guessesAlreadyMade)
{
    Tb nextGuess(-1, '_');
    map<Tb, int> numOccurrencesOfMatch;

    if (guessesAlreadyMade->size() == 0 ||
            possibleAnswers->size() == 1) {
        // Nothing to gain from running minimax here.
        return Tb(0, '0');
    }

    // Count the number of occurrences in possible answers of each match.
    for (PmSet::const_iterator iterPerm = possibleAnswers->begin();
         iterPerm != possibleAnswers->end();
         ++iterPerm) {
	int i = 0;
        for (Pm::const_iterator iterChar = iterPerm->begin();
            iterChar != iterPerm->end();
            ++iterChar) {
            ++numOccurrencesOfMatch[Tb(i++, *iterChar)];
        }
    }

    // Ideally a pairing occurs in 1/2 of all possible answers.
    // The worst-case response is always the complement of the
    // best-case response. We want to optimize the worst case, which
    // occurs when both yes/no cut the remaining solutions in half.
    long optimalNumOccurrences = possibleAnswers->size() / 2;
    long closestNumOccurrences = possibleAnswers->size() + 1;

    for (map<Tb, int>::const_iterator it = numOccurrencesOfMatch.begin();
        it != numOccurrencesOfMatch.end();
        ++it) {

        if (!guessesAlreadyMade->contains(it->first) &&
                (optimalNumOccurrences - it->second) <
                abs(optimalNumOccurrences - closestNumOccurrences)) {
            nextGuess = it->first;
            closestNumOccurrences = it->second;
        }
    }

    return nextGuess;
}

Pm getNextGuessUsingMinimax(PmSet* possibleAnswers, PmSet* guessesAlreadyMade)
{
    // Spin up threads to run minimax in parallel.
    map<Pm, int>* bestGuessFromEachThread = new map<Pm, int>();
    mutex* writeLock = new mutex();
    vector<thread> minimaxThreads;
    vector<ArgsForMinimaxThread*> argsForMinimaxThreads;
    PmSet** chunksToEvaluate;

    if (possibleAnswers->size() > START_PART_MM) {
        // Select the best guess from a fixed pool of possibilities.
        PmSet* fixedPoolToEvaluate = new PmSet();
        fixedPoolToEvaluate->populateFromFile(POOL_FILENAME);
        chunksToEvaluate = fixedPoolToEvaluate->copyIntoChunks(NUM_THREADS);
        delete fixedPoolToEvaluate;
    } else if (possibleAnswers->size() > START_FULL_MM) {
        // Select the best guess from the possible remaining solutions.
        chunksToEvaluate = possibleAnswers->copyIntoChunks(NUM_THREADS);
    } else {
        // Select the best guess from all possible permutations.
        cout << "    Full minimax... May take up to one minute." << endl;
        PmSet* allPossible = new PmSet();
        allPossible->populateAll();
        chunksToEvaluate = allPossible->copyIntoChunks(NUM_THREADS);
        delete allPossible;
    }

    for (int id = 0; id < NUM_THREADS; ++id) {
        ArgsForMinimaxThread* args = new ArgsForMinimaxThread(
                id,
                possibleAnswers,
                chunksToEvaluate[id],
                guessesAlreadyMade,
                bestGuessFromEachThread,
                writeLock);
        argsForMinimaxThreads.push_back(args);
        minimaxThreads.push_back(thread(getBestGuessFromSubset, args));
    }

    // Wait for all threads to finish up.
    for (vector<thread>::iterator it = minimaxThreads.begin();
         it != minimaxThreads.end();
         ++it) {
        it->join();
    }

    // Find the best guess in the set bestGuessFromEachThread.
    Pm bestGuess("");
    long numRemainingAfterBestGuess = possibleAnswers->size();
    for (map<Pm, int>::const_iterator it = bestGuessFromEachThread->begin();
         it != bestGuessFromEachThread->end();
         ++it) {
        Pm guess = it->first;
        int numRemainingAfterGuess = it->second;

        if (numRemainingAfterGuess < numRemainingAfterBestGuess) {
            bestGuess = guess;
            numRemainingAfterBestGuess = numRemainingAfterGuess;
        }
    }

    // Clean up allocated args, chunks, map, and mutex.
    for (vector<ArgsForMinimaxThread*>::iterator it = argsForMinimaxThreads.begin();
         it != argsForMinimaxThreads.end();
         ++it) {
        delete *it;
    }
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (chunksToEvaluate && chunksToEvaluate[i]) {
            delete chunksToEvaluate[i];
        }
    }
    if (chunksToEvaluate) {
        delete chunksToEvaluate;
    }
    delete bestGuessFromEachThread;
    delete writeLock;

    return bestGuess;
}

void getBestGuessFromSubset(ArgsForMinimaxThread* args)
{
    // Find the query in possibleGuesses which eliminates the most possible answers.
    Pm bestGuess = DIGITS;
    long numRemainingAfterBestGuess = args->_possibleAnswers->size();

    for (PmSet::const_iterator possibleGuess = args->_possibleGuesses->begin();
         possibleGuess != args->_possibleGuesses->end();
         ++possibleGuess) {
        Pm guess = *possibleGuess;

        if (!args->_guessesAlreadyMade->contains(guess)) {
            vector<int> numRemainingGivenResponse(PERM_LENGTH + 1, 0);
            // Check how many possibilities would remain after guessing guess
            for (PmSet::const_iterator it = args->_possibleAnswers->begin();
                 it != args->_possibleAnswers->end();
                 ++it) {
                numRemainingGivenResponse[numInCommon(guess, *it)] += 1;
            }
            // Find the # remaining in the worst-case scenario.
            int numRemainingAfterWorstResponse =
                    *max_element(numRemainingGivenResponse.begin(),
                                 numRemainingGivenResponse.end());
            // If this is lowest worst-case #remaining, set low water mark.
            if (numRemainingAfterWorstResponse < numRemainingAfterBestGuess) {
                bestGuess = guess;
                numRemainingAfterBestGuess = numRemainingAfterWorstResponse;
            }
        }
    }

    // After finding best guess in the chunk, write result to the shared dictionary.
    args->_writeLock->lock();
    args->_bestGuesses->insert(pair<Pm, int>(bestGuess, numRemainingAfterBestGuess));
    args->_writeLock->unlock();

    return;
}

string getPrintableNames(PerfectMatching const& p,
                         map<int, string>* femaleNames,
                         map<char, string>* maleNames) {
    stringstream printableNamesBuilder;

    int i = 0;
    for (PerfectMatching::const_iterator it = p.begin(); it != p.end(); it++) {
        printableNamesBuilder << "    " <<  femaleNames->at(i++)
                              << " + " << maleNames->at(*it);
        if (it != p.end() - 1)
            printableNamesBuilder << endl;
    }

    return printableNamesBuilder.str();
}

string getPrintableNames(TruthBooth const& truthBooth,
                         map<int, string>* femaleNames,
                         map<char, string>* maleNames) {
    stringstream printableNamesBuilder;

    printableNamesBuilder << "(" << femaleNames->at(truthBooth.index) << ", "
                          << maleNames->at(truthBooth.charAtIndex) << ")";

    return printableNamesBuilder.str();
}

string getPrintableNumbers(PerfectMatching const& p) {
    stringstream printableNumbersBuilder;

    printableNumbersBuilder << "(";
    for (PerfectMatching::const_iterator i = p.begin(); i != p.end(); i++) {
        printableNumbersBuilder << *i;
        if (i != p.end() - 1)
            printableNumbersBuilder << ", ";
    }
    printableNumbersBuilder << ")";

    return printableNumbersBuilder.str();
}

void runAreYouTheOne(Pm const& answer, AytoSettings const* settings)
{
    TbSet* tbGuessesAlreadyMade = new TbSet(); // Queries submitted in truth booth.
    PmSet* pmGuessesAlreadyMade = new PmSet(); // Queries submitted in perfect matching.
    PmSet* possibleAnswers = new PmSet();      // Remaining possibilities for answer.
    possibleAnswers->populateAll();

    if (!settings->_isInteractiveMode) {
        if (settings->_isPrintNumbersMode) {
            cout << "Answer: " << getPrintableNumbers(answer) << endl;
        } else {
            cout << "Answer:" << endl
                 << getPrintableNames(answer, settings->_femaleNames, settings->_maleNames)
                 << endl;
        }
    }

    while (true) {
        // Submit a single Tb to the Truth Booth.
        cout << "End of Week " << (tbGuessesAlreadyMade->size() + 1) << endl;
        Tb nextTbGuess = getNextTruthBoothGuess(possibleAnswers, tbGuessesAlreadyMade);
        tbGuessesAlreadyMade->add(nextTbGuess);
        bool isPairCorrect;
        if (settings->_isInteractiveMode) {
            string userInput;
            do {
                if (settings->_isPrintNumbersMode) {
                    cout << "Question: Is there a '" << nextTbGuess.charAtIndex
                         << "' in position " << nextTbGuess.index
                         << " (indexed starting with 0)? [yes/no]" << endl;
                } else {
                    cout << "Question: Are " << settings->_femaleNames->at(nextTbGuess.index)
                         << " and " << settings->_maleNames->at(nextTbGuess.charAtIndex)
                         << " matched together? [yes/no]" << endl;
                }
                cin >> userInput;
                isPairCorrect = userInput[0] == 'y' || userInput[0] == 'Y';
            } while (userInput[0] != 'y' && userInput[0] != 'n' &&
                     userInput[0] != 'Y' && userInput[0] != 'N');
        } else {
            isPairCorrect = nextTbGuess.isContainedIn(answer);
        }
        possibleAnswers->filter(nextTbGuess, isPairCorrect);
        if (settings->_isVerboseMode) {
            cout << "  * Truth Booth: " << flush;
            if (settings->_isPrintNumbersMode) {
                cout << getPrintableNumbers(nextTbGuess.toString()) << endl;
            } else {
                cout << getPrintableNames(nextTbGuess,
                                          settings->_femaleNames,
                                          settings->_maleNames)
                     << endl;
            }
            cout << "  > Feedback: " << (isPairCorrect ? "Yes" : "No") << endl;
            cout << "    Now " << possibleAnswers->size() << " remaining." << endl;
        }

        // Submit a full PM as the Perfect Matching.
        Pm nextPmGuess = getNextPerfectMatchingGuess(possibleAnswers, pmGuessesAlreadyMade);
        pmGuessesAlreadyMade->add(nextPmGuess);
        int numCorrect;
        if (settings->_isInteractiveMode) {
            do {
                if (settings->_isPrintNumbersMode) {
                    cout << "Question: How many of the following "
                         << "are in the correct spot?" << endl;
                    cout << getPrintableNumbers(nextPmGuess) << endl;
                } else {
                    cout << "Question: How many of the following "
                         << "matches are correct?" << endl;
                    cout << getPrintableNames(nextPmGuess,
                                              settings->_femaleNames,
                                              settings->_maleNames)
                         << endl;
                }
                cin >> numCorrect;
            } while (numCorrect < 0 || numCorrect > PERM_LENGTH);
        } else {
            numCorrect = numInCommon(nextPmGuess, answer);
        }

        possibleAnswers->filter(nextPmGuess, numCorrect);
        if (settings->_isVerboseMode) {
            cout << "  * Perfect Matching: " << flush;
            if (settings->_isPrintNumbersMode) {
                cout << getPrintableNumbers(nextPmGuess) << endl;
            } else {
                cout << endl << getPrintableNames(nextPmGuess,
                                                  settings->_femaleNames,
                                                  settings->_maleNames)
                     << endl;
            }
            cout << "  > Feedback: " << numCorrect << " correct" << endl;
            cout << "    Now " << possibleAnswers->size() << " remaining." << endl;
        }
        if (numCorrect == PERM_LENGTH) {
            break;
        }
        if (settings->_isInteractiveMode && possibleAnswers->size() == 0) {
            cout << "Sorry, that's not a possible combination of responses."
                 << endl << "Please try again." << endl;
            break;
        }
    }

    // Print a summary of the game, the guesses made in each week.
    cout << "Results:" << endl;
    for (int i = 0; i < tbGuessesAlreadyMade->size(); ++i) {
        int intWidth = (tbGuessesAlreadyMade->size() < 10 ? 1 : 2);
        cout << "[Week " << setw(intWidth) << (i + 1) << "] ";
        if (settings->_isPrintNumbersMode) {
            cout << getPrintableNumbers(tbGuessesAlreadyMade->get(i).toString())
                 << ", "
                 << getPrintableNumbers(pmGuessesAlreadyMade->get(i))
                 << endl;
        } else {
            cout << getPrintableNames(tbGuessesAlreadyMade->get(i),
                                      settings->_femaleNames,
                                      settings->_maleNames)
                 << endl
                 << getPrintableNames(pmGuessesAlreadyMade->get(i),
                                      settings->_femaleNames,
                                      settings->_maleNames)
                 << endl;
        }
    }

    delete tbGuessesAlreadyMade;
    delete pmGuessesAlreadyMade;
    delete possibleAnswers;
}

Pm getRandomPerfectMatching() {
    Pm p = DIGITS;
    int last = (int) p.size();
    while (--last > 0) {
        int i = rand() % last;
        char tmp = p[last];
        p[last] = p[i];
        p[i] = tmp;
    }
    return p;
}
