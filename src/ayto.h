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
#include "Perms.h"
#include "Match.h"
#include "Matches.h"
using namespace std;

#define DIGITS ("0123456789") // Available digits for a permutation.
#define GUESS  ("4579108623") // The fixed guess made in the second turn.
#define NUM_THREADS (10)      // Number of threads to use during minimax.
#define PERM_LENGTH (10)      // Number of elements in a permutation.
#define POOL_FILENAME ("scripts/pool.txt") // Location of fixed guess pool.
#define START_FULL_MM (314)   // Start w/ full minimax when <= this # left.
#define START_PART_MM (10000) // Start partial minimax when <= this # left.

//typedef TruthBooth TB;

struct AreYouTheOneSettings {
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
};

struct ArgsForMinimaxThread {
    int _threadId;                // ID of thread taking these args.
    Perms* _possibleAnswers;      // All permutations still possible to be the answer.
    Perms* _possibleGuesses;      // The chunk of potential queries to evaluate.
    Perms* _guessesAlreadyMade;   // All queries made so far.
    map<PerfectMatching, int>* _bestGuesses; // Where each thread will store it's best query.
    mutex* _writeLock;            // Mutex for the shared _best_queries map.
    ArgsForMinimaxThread(                   // Wordy initializer, but blame it on thread api.
            int threadId,
            Perms* possibleAnswers,
            Perms* possibleGuesses,
            Perms* guessesAlreadyMade,
            map<PerfectMatching, int>* bestGuesses,
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
PerfectMatching getNextGuessUsingMinimax(Perms *, Perms *);
Match getNextTruthBoothGuess(Perms*, Matches*);
PerfectMatching getNextPerfectMatchingGuess(Perms*, Perms*);
void runAreYouTheOne(PM const &, AreYouTheOneSettings const *);

#endif
