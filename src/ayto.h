/**
 * ayto.cpp
 * Christopher Chute
 *
 * Simulates minimax query algorithm for MTV's Are You The One?
 */

#ifndef GUARD_AYTO_H
#define GUARD_AYTO_H

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <map>
#include <mutex>
#include <cstring>
#include <thread>
#include <vector>
#include "PerfectMatching.h"
#include "PerfectMatchingSet.h"
#include "TruthBooth.h"
#include "TruthBoothSet.h"
using namespace std;

#define DIGITS ("0123456789") // Available digits for a permutation.
#define GUESS  ("4579108623") // The fixed guess made in the second turn.
#define NUM_THREADS (10)      // Number of threads to use during minimax.
#define PERM_LENGTH (10)      // Number of elements in a permutation.
#define POOL_FILENAME ("scripts/pool.txt") // Location of fixed guess pool.
#define START_FULL_MM (314)   // Start w/ full minimax when <= this # left.
#define START_PART_MM (10000) // Start partial minimax when <= this # left.

typedef PerfectMatching Pm;
typedef PerfectMatchingSet PmSet;
typedef TruthBooth Tb;
typedef TruthBoothSet TbSet;

typedef struct AreYouTheOneSettings {
    bool _isAllPermutationsMode;
    bool _isReadFromFileMode;
    bool _isInteractiveMode;
    bool _isVerboseMode;
    std::string _fileToRead;
    AreYouTheOneSettings() :
            _isAllPermutationsMode(false),
            _isReadFromFileMode(false),
            _isInteractiveMode(false),
            _isVerboseMode(true),
            _fileToRead("")
    {}
    bool initializeFromArgs(int argc, char **argv);
} AytoSettings;

struct ArgsForMinimaxThread {
    int _threadId;              // ID of thread taking these args.
    PmSet* _possibleAnswers;    // All permutations still possible to be the answer.
    PmSet* _possibleGuesses;    // The chunk of potential queries to evaluate.
    PmSet* _guessesAlreadyMade; // All queries made so far.
    map<Pm, int>* _bestGuesses; // Where each thread will store it's best query.
    mutex* _writeLock;          // Mutex for the shared _best_queries map.
    ArgsForMinimaxThread(       // Wordy initializer, but blame it on thread api.
            int threadId,
            PmSet* possibleAnswers,
            PmSet* possibleGuesses,
            PmSet* guessesAlreadyMade,
            map<Pm, int>* bestGuesses,
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
Tb getNextTruthBoothGuess(PmSet*, TbSet*);
Pm getNextPerfectMatchingGuess(PmSet*, PmSet*);
void runAreYouTheOne(PerfectMatching const&, AytoSettings const*);

#endif
