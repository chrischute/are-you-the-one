/**
 * TruthBoothSet.h
 * Christopher Chute
 *
 * Data structure and operations for single matches.
 */

#ifndef GUARD_MATCHES_H
#define GUARD_MATCHES_H

#include <algorithm>
#include <vector>
#include "TruthBooth.h"

class TruthBoothSet {
private:
    std::vector<TruthBooth>* data;
public:
    TruthBoothSet();
    ~TruthBoothSet();

    void add(TruthBooth&);
    bool contains(const TruthBooth&);
    TruthBooth& get(int i);
    std::vector<TruthBooth>::size_type size();
};


#endif
