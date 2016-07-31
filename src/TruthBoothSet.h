/**
 * TruthBoothSet.h
 * Christopher Chute
 *
 * Data structure and utilities for container of Truth Booth guesses.
 */

#ifndef GUARD_TRUTH_BOOTH_SET_H
#define GUARD_TRUTH_BOOTH_SET_H

#include <vector>
#include "TruthBooth.h"

class TruthBoothSet {
private:
    std::vector<TruthBooth>* data;
public:
    TruthBoothSet();
    ~TruthBoothSet();

    void add(TruthBooth&);
    bool contains(TruthBooth const &);
    TruthBooth& get(int i);
    std::vector<TruthBooth>::size_type size();
};

#endif
