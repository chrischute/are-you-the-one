/**
 * Matches.h
 * Christopher Chute
 *
 * Data structure and operations for single matches.
 */

#ifndef GUARD_MATCHES_H
#define GUARD_MATCHES_H

#include <algorithm>
#include <vector>
#include "Match.h"

class Matches {
private:
    std::vector<Match>* data;
public:
    Matches();
    ~Matches();

    void add(Match&);
    bool contains(const Match&);
    Match& get(int i);
    std::vector<Match>::size_type size();
};


#endif
