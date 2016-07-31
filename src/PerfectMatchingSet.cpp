/**
 * PerfectMatchingSet.cpp
 * Christopher Chute
 * 
 * Data structure and utilities for container of Perfect Matching guesses.
 */

#include <cassert>
#include <fstream>
#include "PerfectMatchingSet.h"

using std::ifstream;
using std::string;
using std::vector;

typedef PerfectMatching Pm;
typedef TruthBooth Tb;

PerfectMatchingSet::PerfectMatchingSet(){
    this->data = new vector<Pm>();
}

PerfectMatchingSet::PerfectMatchingSet(iterator begin, iterator end) {
    this->data = new vector<Pm>(begin, end);
}

PerfectMatchingSet::~PerfectMatchingSet() {
    delete this->data;
}

void PerfectMatchingSet::add(Pm& p) {
    this->data->push_back(p);
}

PerfectMatchingSet::iterator PerfectMatchingSet::begin() {
    return this->data->begin();
}

bool PerfectMatchingSet::contains(Pm const& p) {
    return find(this->begin(), this->end(), p) != this->end();
}

PerfectMatchingSet::iterator PerfectMatchingSet::end() {
    return this->data->end();
}

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
    return this->data->at(unsigned(i));
}

void PerfectMatchingSet::populateAll() {
    Pm p = "0123456789";
    do {
        this->add(p);
    } while (next_permutation(p.begin(), p.end()));
}

void PerfectMatchingSet::populateFromFile(string filename) {
    Pm s;
    ifstream inputFile(filename);

    while (inputFile >> s) {
        this->add(s);
    }

    inputFile.close();
}

vector<Pm>::size_type PerfectMatchingSet::size() {
    return this->data->size();
}

PerfectMatchingSet** PerfectMatchingSet::copyIntoChunks(int numChunks) {
    PerfectMatchingSet** chunks = new PerfectMatchingSet*[numChunks];
    long chunkSize = this->size() / numChunks;
    long numChunksWithOneExtra = this->size() % numChunks;

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
