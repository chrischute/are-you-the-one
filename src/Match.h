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
    bool isContainedIn(Perm &p) { return p[this->index] == this->charAtIndex; }
    std::string toString() {
        const char charArray[] = {
                static_cast<char>('0' + index),
                charAtIndex
        };
        return std::string(charArray, 2);
    }
    bool operator==(Match const& that) const {
        return this->index == that.index &&
               this->charAtIndex == that.charAtIndex;
    }
    bool operator<(Match const& that) const {
        return 10 * this->index + (this->charAtIndex - '0') <
               10 * that.index + (that.charAtIndex - '0');
    }
};
#endif
