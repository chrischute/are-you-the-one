/**
 * ayto.cpp
 * Christopher Chute
 * 
 * Simulates minimax query algorithm for MTV's Are You The One?
 */

#include "ayto.h"

typedef PerfectMatching PM;

int main(int argc, char **argv) {
    AreYouTheOneSettings* settings = new AreYouTheOneSettings();

    if (settings->initializeFromArgs(argc, argv)) {
        if (settings->_isAllPermutationsMode) {
            cout << "Running on all possible answers." << endl;
            PerfectMatching answer = DIGITS;
            do {
                runAreYouTheOne(answer, settings);
            } while (next_permutation(answer.begin(), answer.end()));
        } else if (settings->_isReadFromFileMode) {
            cout << "Reading answers from " << settings->_fileToRead << "." << endl;
            Perms *answers = new Perms();
            answers->populateFromFile(settings->_fileToRead);
            for (Perms::const_iterator answer = answers->begin();
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
            PerfectMatching answer = DIGITS;
            srand(unsigned(time(NULL)));
            random_shuffle(answer.begin(), answer.end());
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

bool AreYouTheOneSettings::initializeFromArgs(int argc, char **argv) {
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
        } else {
            return false;
        }
    }
    return true;
}

PM getNextPMGuess(Perms *possibleAnswers, Perms *guessesAlreadyMade)
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

Match getNextTruthBoothGuess(Perms* possibleAnswers, Matches* guessesAlreadyMade)
{
    Match nextGuess(-1, '_');
    map<Match, int> numOccurrencesOfMatch;

    if (guessesAlreadyMade->size() == 0 ||
            possibleAnswers->size() == 1) {
        // Nothing to gain from running minimax here.
        return Match(0, '0');
    }

    // Count the number of occurrences in possible answers of each match.
    for (Perms::const_iterator iterPerm = possibleAnswers->begin();
         iterPerm != possibleAnswers->end();
         ++iterPerm) {
        PM::const_iterator firstIterChar = iterPerm->begin();
        for (PM::const_iterator iterChar = iterPerm->begin();
            iterChar != iterPerm->end();
            ++iterChar) {
            ++numOccurrencesOfMatch[Match(iterChar - firstIterChar, *iterChar)];
        }
    }

    // Ideally a pairing occurs in 1/2 of all possible answers.
    // The worst-case response is always the complement of the
    // best-case response. We want to optimize the worst case, which
    // occurs when both yes/no cut the remaining solutions in half.
    long optimalNumOccurrences = possibleAnswers->size() / 2;
    long closestNumOccurrences = possibleAnswers->size() + 1;

    for (map<Match, int>::const_iterator it = numOccurrencesOfMatch.begin();
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

PM getNextGuessUsingMinimax(Perms* possibleAnswers, Perms* guessesAlreadyMade)
{
    // Spin up threads to run minimax in parallel.
    map<PM, int>* bestGuessFromEachThread = new map<PM, int>();
    mutex *writeLock = new mutex();
    vector<thread> minimaxThreads;
    vector<ArgsForMinimaxThread*> argsForMinimaxThreads;
    Perms** chunksToEvaluate;

    if (possibleAnswers->size() > START_PART_MM) {
        // Select the best guess from a fixed pool of possibilities.
        Perms* fixedPoolToEvaluate = new Perms();
        fixedPoolToEvaluate->populateFromFile(POOL_FILENAME);
        chunksToEvaluate = fixedPoolToEvaluate->copyIntoChunks(NUM_THREADS);
        delete fixedPoolToEvaluate;
    } else if (possibleAnswers->size() > START_FULL_MM) {
        // Select the best guess from the possible remaining solutions.
        chunksToEvaluate = possibleAnswers->copyIntoChunks(NUM_THREADS);
    } else {
        // Select the best guess from all possible permutations.
        cout << "    Full minimax... May take up to one minute." << endl;
        Perms* allPossible = new Perms();
        allPossible->populateAll();
        chunksToEvaluate = allPossible->copyIntoChunks(NUM_THREADS);
        delete allPossible;
    }

    for (int id = 0; id < NUM_THREADS; ++id) {
        ArgsForMinimaxThread *args = new ArgsForMinimaxThread(
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
    PM bestGuess("");
    long numRemainingAfterBestGuess = possibleAnswers->size();
    for (map<PM, int>::const_iterator it = bestGuessFromEachThread->begin();
         it != bestGuessFromEachThread->end();
         ++it) {
        PM guess = it->first;
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
    PM bestGuess = DIGITS;
    long numRemainingAfterBestGuess = args->_possibleAnswers->size();

    for (Perms::const_iterator possibleGuess = args->_possibleGuesses->begin();
         possibleGuess != args->_possibleGuesses->end();
         ++possibleGuess) {
        PM guess = *possibleGuess;

        if (!args->_guessesAlreadyMade->contains(guess)) {
            vector<int> numRemainingGivenResponse(PERM_LENGTH + 1, 0);
            // Check how many possibilities would remain after guessing guess
            for (Perms::const_iterator it = args->_possibleAnswers->begin();
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
    args->_bestGuesses->insert(pair<PM, int>(bestGuess, numRemainingAfterBestGuess));
    args->_writeLock->unlock();

    return;
}

void runAreYouTheOne(PM const& answer, AreYouTheOneSettings const* settings)
{
    Perms* possibleAnswers = new Perms(); // Remaining possibilities for answer.
    possibleAnswers->populateAll();
    Matches* tbGuessesAlreadyMade = new Matches(); // Queries submitted in truth booth.
    Perms* pmGuessesAlreadyMade = new Perms(); // Queries submitted in perfect matching.

    if (!settings->_isInteractiveMode) {
        cout << "Answer " << answer << endl;
    }

    while (true) {
        // Submit a single Match to the Truth Booth.
        cout << "End of Week " << (tbGuessesAlreadyMade->size() + 1) << endl;
        Match nextTbGuess = getNextTruthBoothGuess(possibleAnswers, tbGuessesAlreadyMade);
        tbGuessesAlreadyMade->add(nextTbGuess);
        bool isPairCorrect;
        if (settings->_isInteractiveMode) {
            string userInput;
            do {
                cout << "Question: Is there a '" << nextTbGuess.charAtIndex
                     << "' in position " << nextTbGuess.index
                     << " (indexed starting with 0)? [yes/no]" << endl;
                cin >> userInput;
                isPairCorrect = userInput[0] == 'y' || userInput[0] == 'Y';
            } while (userInput[0] != 'y' && userInput[0] != 'n' &&
                     userInput[0] != 'Y' && userInput[0] != 'N');
        } else {
            isPairCorrect = nextTbGuess.isContainedIn(answer);
        }
        possibleAnswers->filter(nextTbGuess, isPairCorrect);
        if (settings->_isVerboseMode) {
            cout << "  * Truth Booth: " << nextTbGuess.toString() << endl;
            cout << "    Now " << possibleAnswers->size() << " remaining." << endl;
        }

        // Submit a full PM as the Perfect Matching.
        PM nextPMGuess = getNextPMGuess(possibleAnswers, pmGuessesAlreadyMade);
        pmGuessesAlreadyMade->add(nextPMGuess);
        int numCorrect;
        if (settings->_isInteractiveMode) {
            do {
                cout << "Question: How many of the following "
                     << "are in the correct spot?" << endl
                     << getPrintablePerm(nextPMGuess) << endl;
                cin >> numCorrect;
            } while (numCorrect < 0 || numCorrect > PERM_LENGTH);
        } else {
            numCorrect = numInCommon(nextPMGuess, answer);
        }

        possibleAnswers->filter(nextPMGuess, numCorrect);
        if (settings->_isVerboseMode) {
            cout << "  * Perfect Matching: "
                 << getPrintablePerm(nextPMGuess) << endl;
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
        cout << "[Week " << setw(intWidth) << (i + 1) << "] "
             << tbGuessesAlreadyMade->get(i).toString() << ", "
             << getPrintablePerm(pmGuessesAlreadyMade->get(i)) << "." << endl;
    }

    delete possibleAnswers;
    delete tbGuessesAlreadyMade;
    delete pmGuessesAlreadyMade;
}
