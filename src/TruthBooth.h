/**
 * TruthBooth.h
 * Christopher Chute
 *
 * Data structure for a single match.
 */

#ifndef GUARD_TRUTH_BOOTH_H
#define GUARD_TRUTH_BOOTH_H

#include <sstream>
#include "PerfectMatching.h"

class TruthBooth {
public:
    int index;
    char charAtIndex;

    TruthBooth() : index(0), charAtIndex('0') { }
    TruthBooth(int i, char c) : index(i), charAtIndex(c) { }
    TruthBooth(long i, char c) : index((int) i), charAtIndex(c) { }
    bool isContainedIn(PerfectMatching const& p) { return p[this->index] == this->charAtIndex; }
    std::string toString() {
        std::stringstream tbStringBuilder;
        tbStringBuilder << "(" << this->index
                << ", " << this->charAtIndex << ")";
        return tbStringBuilder.str();
    }
    bool operator==(TruthBooth const& that) const {
        return this->index == that.index &&
               this->charAtIndex == that.charAtIndex;
    }
    bool operator<(TruthBooth const& that) const {
        return 10 * this->index + (this->charAtIndex - '0') <
               10 * that.index + (that.charAtIndex - '0');
    }
};

#endif
