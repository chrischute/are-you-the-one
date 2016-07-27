//
// Created by Christopher Chute on 7/27/16.
//

#ifndef GUARD_AYTO_H
#define GUARD_AYTO_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <mutex>
#include <cstring>
#include <thread>
#include <vector>
#include "Perm.h"
#include "Perms.h"
#include "Match.h"
#include "Matches.h"
using namespace std;

#define DIGITS ("0123456789") // Available digits for a permutation.
#define DUMMY  ("9352741680") // Dummy answer, used as default when no args.
#define GUESS  ("4579108623") // The fixed guess made in the second turn.
#define PERM_LENGTH (10)      // Number of elements in a permutation.
#define POOL_FILENAME ("scripts/pool.txt") // Location of fixed guess pool.
#define START_FULL_MM (314)   // Start w/ full minimax when <= this # left.
#define START_PART_MM (10000) // Start partial minimax when <= this # left.

typedef struct targs Thread_args;
struct targs {
    int _id;                       // ID of thread taking these args.
    Perms* _poss_answers;          // All permutations still possible to be the answer.
    Perms* _poss_queries;          // The chunk of potential queries to evaluate.
    Perms* _queries_made;          // All queries made so far.
    map<Perm, int>* _best_queries; // Where each thread will store it's best query.
    mutex* _mutex;                 // Mutex for the shared _best_queries map.
};

void minimaxPerThread(Thread_args *args);
Perm minimax(Perms* poss, Perms* queries_made);
Match getNextTruthBoothGuess(Perms* poss, Matches* queries_made);
Perm getNextPerfectMatchingGuess(Perms* poss, Perms* queries_made);
void runAyto(Perm answer); // TODO: Interactive flag, verbose flag.
void runAytoInteractive();
bool isRunAllMode(int argc, char **argv);
bool isRunFileMode(int argc, char **argv);
bool isRunInteractiveMode(int argc, char **argv);

#endif
