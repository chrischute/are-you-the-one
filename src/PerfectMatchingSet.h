/**
 * PerfectMatchingSet.h
 * Christopher Chute
 * 
 * Data structure and operations for permutations.
 */

#ifndef GUARD_PERMS_H
#define GUARD_PERMS_H

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include "PerfectMatching.h"
#include "TruthBooth.h"

class PerfectMatchingSet {
private:
    std::vector<PerfectMatching>* data;
public:
    // PerfectMatchingSet::iterator: Iterates over the data in set of permutations.
    typedef std::vector<PerfectMatching>::iterator iterator;
    // PerfectMatchingSet::const_iterator: Iterates over data vector, read only.
    typedef std::vector<PerfectMatching>::const_iterator const_iterator;

    // PerfectMatchingSet(): Initialize empty set of permutations.
    PerfectMatchingSet();
    // PerfectMatchingSet(iterator, iterator): Initialize a copy from another set of permutations.
    PerfectMatchingSet(iterator begin, iterator end);
    // ~PerfectMatchingSet: Destroy all associated data.
    ~PerfectMatchingSet();

    // add: add an element to the set of permutations.
    void add(PerfectMatching&);
    // begin: get the iterator at the front of this set of permutations.
    iterator begin();
    // contains: true if and only if this contains specified permutation.
    bool contains(PerfectMatching const&);
    // end: get the iterator one past the last element of this set of permutations.
    iterator end();
    // filter: filter for only permutations with n_correct matches.
    void filter(PerfectMatching const&, int);
    // filter: filter for only that have character m.second at index m.first.
    void filter(TruthBooth const& m, bool isMatch);
    // get: get the element at index i, not checking for data bounds.
    PerfectMatching& get(int i);
    // populateAll(): Fills with all permutations of the digits 0 through 9.
    void populateAll();
    // populateFromFile(std::string): Fills with all permutations in the file stream.
    // Format: File should contain a single 10-digit number per line, nothing else.
    void populateFromFile(std::string filename);
    // size: get the number of elements in the permutation set.
    std::vector<PerfectMatching>::size_type size();
    // splitIntoChunks: split into numChunks evenly sized chunks.
    PerfectMatchingSet** copyIntoChunks(int);
};
#endif
