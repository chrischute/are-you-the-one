/**
 * Perms.h
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
#include "Perm.h"
#include "Match.h"

class Perms {
private:
    std::vector<Perm>* data;
public:
    // Perms::iterator: Iterates over the data in set of permutations.
    typedef std::vector<Perm>::iterator iterator;
    // Perms::const_iterator: Iterates over data vector, read only.
    typedef std::vector<Perm>::const_iterator const_iterator;

    // Perms(): Initialize empty set of permutations.
    Perms();
    // Perms(iterator, iterator): Initialize a copy from another set of permutations.
    Perms(iterator begin, iterator end);
    // ~Perms: Destroy all associated data.
    ~Perms();

    // add: add an element to the set of permutations.
    void add(Perm&);
    // begin: get the iterator at the front of this set of permutations.
    iterator begin();
    // contains: true if and only if this contains specified permutation.
    bool contains(Perm&);
    // end: get the iterator one past the last element of this set of permutations.
    iterator end();
    // filter: filter for only permutations with n_correct matches.
    void filter(Perm&, int);
    // filter: filter for only that have character m.second at index m.first.
    void filter(Match& m, bool isMatch);
    // get: get the element at index i, not checking for data bounds.
    Perm& get(int i);
    // populateAll(): Fills with all permutations of the digits 0 through 9.
    void populateAll();
    // populateFromFile(ifstream): Fills with all permutations in the file stream.
    // Format: File should contain a single 10-digit number per line, nothing else.
    void populateFromFile(std::string filename);
    // sample: peek at some element in the permutation set.
    Perm& sample();
    // size: get the number of elements in the permutation set.
    std::vector<Perm>::size_type size();
};
#endif