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

void getBestGuessFromSubset(ArgsForMinimaxThread *);
Pm getNextGuessUsingMinimax(PmSet*, PmSet*);
Pm getNextPerfectMatchingGuess(PmSet*, PmSet*);
Tb getNextTruthBoothGuess(PmSet*, TbSet*);
void runAreYouTheOne(PerfectMatching const&, AytoSettings const*);
std::string getPrintableNames(PerfectMatching const&,
                              std::map<int, std::string>*,
                              std::map<char, std::string>*);
std::string getPrintableNames(TruthBooth const&,
                              std::map<int, std::string>*,
                              std::map<char, std::string>*);
std::string getPrintableNumbers(PerfectMatching const&);
void shufflePerfectMatching(Pm &p);

#endif
