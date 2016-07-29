/**
 * Match.h
 * Christopher Chute
 *
 * Data structure for a single match.
 */

#ifndef GUARD_MATCH_H
#define GUARD_MATCH_H

#include <algorithm>
#include <sstream>
#include "Perm.h"

class Match {
public:
    int index;
    char charAtIndex;

    Match() : index(0), charAtIndex('0') { }
    Match(int i, char c) : index(i), charAtIndex(c) { }
    Match(long i, char c) : index((int) i), charAtIndex(c) { }
    bool isContainedIn(Perm const& p) { return p[this->index] == this->charAtIndex; }
    std::string toString() {
        std::stringstream builder;
        builder << "(" << this->index
                << ", " << this->charAtIndex << ")";
        return builder.str();
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
