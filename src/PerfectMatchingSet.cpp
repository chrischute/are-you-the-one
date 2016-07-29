/**
 * PerfectMatchingSet.cpp
 * Christopher Chute
 * 
 * Data structure and operations for permutations.
 */

#include <cassert>
#include "PerfectMatchingSet.h"
using namespace std;

typedef PerfectMatching Pm;
typedef TruthBooth Tb;

// PerfectMatchingSet(): Initialize empty set of permutations.
PerfectMatchingSet::PerfectMatchingSet(){
    this->data = new vector<Pm>();
}

// PerfectMatchingSet(PerfectMatchingSet::iterator begin, PerfectMatchingSet::iterator end): init from vector
PerfectMatchingSet::PerfectMatchingSet(iterator begin, iterator end) {
    this->data = new vector<Pm>(begin, end);
}

// ~PerfectMatchingSet: Destroy all associated data.
PerfectMatchingSet::~PerfectMatchingSet() {
    delete this->data;
}

// add: add an element to the set of permutations.
void PerfectMatchingSet::add(Pm& p) {
    this->data->push_back(p);
}

// begin: get the iterator at the front of this set of permutations.
PerfectMatchingSet::iterator PerfectMatchingSet::begin() {
    return this->data->begin();
}

// contains: true if and only if this contains specified permutation.
bool PerfectMatchingSet::contains(Pm const& p) {
    return find(this->begin(), this->end(), p) != this->end();
}

// end: get the iterator one past the last element of this set of permutations.
PerfectMatchingSet::iterator PerfectMatchingSet::end() {
    return this->data->end();
}

// filter: filter for only permutations with n matches in common with p.
void PerfectMatchingSet::filter(Pm const& p, int n) {
    vector<Pm>* newData = new vector<Pm>();
    for (PerfectMatchingSet::iterator it = this->begin(); it != this->end(); ++it) {
        if (numInCommon(p, *it) == n) {
            newData->push_back(*it);
        }
    }

    delete this->data;
    this->data = newData;
}

void PerfectMatchingSet::filter(Tb const& m, bool isMatch) {
    vector<Pm>* newData = new vector<Pm>();
    for (PerfectMatchingSet::iterator it = this->begin(); it != this->end(); ++it) {
        if (isMatch == ((*it)[m.index] == m.charAtIndex)) {
            newData->push_back(*it);
        }
    }

    delete this->data;
    this->data = newData;
}

Pm& PerfectMatchingSet::get(int i) {
    return this->data->at(i);
}

// populateAll(int): Fills with all permutations of the provided length.
void PerfectMatchingSet::populateAll() {
    Pm p = "0123456789";
    do {
        this->add(p);
    } while (next_permutation(p.begin(), p.end()));
}

// populateFromFile(string): Fills with all permutations in the file at provided path.
void PerfectMatchingSet::populateFromFile(string filename) {
    Pm s;
    ifstream inputFile(filename);

    while (inputFile >> s) {
        this->add(s);
    }

    inputFile.close();
}

// size: get the number of elements in the permutation set.
vector<Pm>::size_type PerfectMatchingSet::size() {
    return this->data->size();
}

// copyIntoChunks: copy into numChunks evenly sized chunks.
// User is responsible for freeing returned chunks.
PerfectMatchingSet** PerfectMatchingSet::copyIntoChunks(int numChunks) {
    PerfectMatchingSet** chunks = new PerfectMatchingSet*[numChunks];
    int chunkSize = this->size() / numChunks;
    int numChunksWithOneExtra = this->size() % numChunks;

    int i = 0;
    for (PerfectMatchingSet::iterator beginChunk = this->begin(), endChunk = this->begin();
         beginChunk != this->end();
         beginChunk = endChunk) {
        endChunk += chunkSize;
        if (numChunksWithOneExtra-- > 0) {
            ++endChunk;
        }

        chunks[i++] = new PerfectMatchingSet(beginChunk, endChunk);
    }
    assert(i == numChunks);

    return chunks;
}
