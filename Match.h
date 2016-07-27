/**
 * Match.h
 * Christopher Chute
 *
 * Data structure for a single match.
 */

#ifndef GUARD_MATCH_H
#define GUARD_MATCH_H

#include <algorithm>
#include "Perm.h"

class Match {
public:
    int index;
    char charAtIndex;
    Match() : index(0), charAtIndex('0') { }
    Match(int i, char c) : index(i), charAtIndex(c) { }
    int isIn(Perm& p) { return p[this->index] == this->charAtIndex ? 1 : 0; }
    Perm toPerm() {
        const char perm[] = {
                static_cast<char>('0' + index),
                charAtIndex
        };
        return string(perm, 2);
    }
};
#endif
