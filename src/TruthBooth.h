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
    bool operator==(const TruthBooth& that) const {
        return this->index == that.index &&
               this->charAtIndex == that.charAtIndex;
    }
    bool operator<(const TruthBooth& that) const {
        return 10 * this->index + (this->charAtIndex - '0') <
               10 * that.index + (that.charAtIndex - '0');
    }

    /**
     * Check whether this TruthBooth is a match within pm.
     * @param pm PerfectMatching to check for this match.
     * @return True if this TruthBooth is a match in pm, false otherwise.
     */
    bool isContainedIn(const PerfectMatching& pm) const {
        return pm[this->index] == this->charAtIndex;
    }

    /**
     * Convert this TruthBooth to a length-two string: index and charAtIndex.
     * @return String of length two, corresponding to index and charAtIndex.
     */
    std::string toString() const {
        const char chars[] = {
                static_cast<char>('0' + index),
                this->charAtIndex
        };
        return std::string(chars, 2);
    }
};

#endif
