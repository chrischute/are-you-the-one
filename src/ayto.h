/**
 * ayto.h
 * Christopher Chute
 *
 * Guess generator for MTV's Are You The One?
 */

#ifndef GUARD_AYTO_H
#define GUARD_AYTO_H

#include <map>
#include <mutex>
#include "PerfectMatching.h"
#include "PerfectMatchingSet.h"
#include "TruthBooth.h"
#include "TruthBoothSet.h"

using std::map;
using std::mutex;

typedef PerfectMatching Pm;
typedef PerfectMatchingSet PmSet;
typedef TruthBooth Tb;
typedef TruthBoothSet TbSet;

/**
 * Settings for simulating Are You The One?
 */
typedef struct AreYouTheOneSettings {
    std::string _fileToRead;
    bool _isAllPermutationsMode;
    bool _isInteractiveMode;
    bool _isPrintNumbersMode;
    bool _isReadFromFileMode;
    bool _isVerboseMode;
    std::map<int, std::string>* _femaleNames; // Index maps to female name.
    std::map<char, std::string>* _maleNames;  // Char at index maps to male name.
    AreYouTheOneSettings() :
            _fileToRead(""),
            _isAllPermutationsMode(false),
            _isInteractiveMode(false),
            _isPrintNumbersMode(false),
            _isReadFromFileMode(false),
            _isVerboseMode(true),
            _femaleNames(nullptr),
            _maleNames(nullptr)
    {}
    ~AreYouTheOneSettings() {
        if (!_isPrintNumbersMode) {
            delete _femaleNames;
            delete _maleNames;
        }
    }
    bool initializeFromArgs(int argc, char **argv);
} AytoSettings;

/**
 * Arguments passed to a single thread during minimax, when each
 * thread finds the best guess over a subset of all possible guesses.
 */
struct ArgsForMinimaxThread {
    int _threadId;                   // ID of thread taking these args.
    PmSet* _possibleAnswers;         // All permutations still possible to be the answer.
    PmSet* _possibleGuesses;         // The chunk of potential queries to evaluate.
    PmSet* _guessesAlreadyMade;      // All queries made so far.
    std::map<Pm, int>* _bestGuesses; // Where each thread will store it's best query.
    std::mutex* _writeLock;          // Mutex for the shared _best_queries map.
    ArgsForMinimaxThread(            // Wordy initializer, but blame it on thread api.
            int threadId,
            PmSet* possibleAnswers,
            PmSet* possibleGuesses,
            PmSet* guessesAlreadyMade,
            std::map<Pm, int>* bestGuesses,
            mutex* writeLock)
            :
            _threadId(threadId),
            _possibleAnswers(possibleAnswers),
            _possibleGuesses(possibleGuesses),
            _guessesAlreadyMade(guessesAlreadyMade),
            _bestGuesses(bestGuesses),
            _writeLock(writeLock)
    {}
};

/**
 * Get the best guess from a subset of all possible guesses. This is the
 * job passed to a single thread during minimax.
 * @param argsForMinimaxThread Contains chunk to evaluate, info needed to
 * determine the best guess, and pointers for placing the best guess when found.
 */
void getBestGuessFromSubset(ArgsForMinimaxThread *argsForMinimaxThread);

/**
 * Apply minimax algorithm to find the best next PerfectMatching to guess.
 * @param possibleAnswers Remaining possibilities for the answer.
 * @param guessesAlreadyMade Guesses already submitted in Perfect Matching stage.
 * @return The best guess as determined by the minimax algorithm.
 */
Pm getNextGuessUsingMinimax(PmSet* possibleAnswers, PmSet* guessesAlreadyMade);

/**
 * Find the best next PerfectMatching to guess. May not require minimax.
 * @param possibleAnswers Remaining possibilities for the answer.
 * @param guessesAlreadyMade Guesses already submitted in Perfect Matching stage.
 * @return Best guess as determined by the minimax algorithm.
 */
Pm getNextPerfectMatchingGuess(PmSet* possibleAnswers, PmSet* guessesAlreadyMade);

/**
 * Find the best next TruthBooth to guess. Chooses the pair which occurs in
 * possible answers in proportion closest to 50%. We want to minimize number
 * remaining in the worst-case scenario.
 * @param possibleAnswers Remaining possibilities for the answer.
 * @param guessesAlreadyMade Guesses already submitted in Perfect Matching stage.
 * @return Best TruthBooth guess as determined by the minimax algorithm.
 */
Tb getNextTruthBoothGuess(PmSet* possibleAnswers, TbSet* guessesAlreadyMade);

/**
 * Run a single season of Are You The One.
 * @param answer Hidden answer, i.e., the matching of contestants.
 * @param settings Settings for the simulator, e.g., interactive and verbose.
 */
void runAreYouTheOne(PerfectMatching const& answer, AytoSettings const* settings);

/**
 * Convert a PerfectMatching to a printable representation using names of
 * real-life contestants.
 * @param perfectMatching Full matching to convert to a printable string.
 * @param femaleNames Position-to-name of girl mapping for a permutation.
 * @param maleNames Char-to-name of guy mapping for a permutation.
 * @return String to be logged to the console.
 */
std::string getPrintableNames(PerfectMatching const& perfectMatching,
                              std::map<int, std::string>* femaleNames,
                              std::map<char, std::string>* maleNames);

/**
 * Convert a TruthBooth guess to a printable representation using names of
 * real-life contestants.
 * @param perfectMatching Full matching to convert to a printable string.
 * @param femaleNames Position-to-name of girl mapping for a permutation.
 * @param maleNames Char-to-name of guy mapping for a permutation.
 * @return String to be logged to the console.
 */
std::string getPrintableNames(TruthBooth const& truthBooth,
                              std::map<int, std::string>* femaleNames,
                              std::map<char, std::string>* maleNames);

/**
 * Convert a PerfectMatching to a printable representation using numbers.
 * @param perfectMatching Full matching to convert to a printable string.
 * @return String to be logged to the console.
 */
std::string getPrintableNumbers(PerfectMatching const& perfectMatching);

/**
 * Get a random perfect matching.
 * @return Random perfect matching, permutation of 0123456789.
 */
Pm getRandomPerfectMatching();

#endif
