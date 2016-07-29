/**
 * Perms.cpp
 * Christopher Chute
 * 
 * Data structure and operations for permutations.
 */

#include <cassert>
#include "Perms.h"

using namespace std;

// Perms(): Initialize empty set of permutations.
Perms::Perms() {
    data = new vector<Perm>();
}

// Perms(Perms::iterator begin, Perms::iterator end): init from vector
Perms::Perms(Perms::iterator begin, Perms::iterator end) {
    data = new vector<Perm>(begin, end);
}

// ~Perms: Destroy all associated data.
Perms::~Perms() {
    delete data;
}

// add: add an element to the set of permutations.
void Perms::add(Perm& p) {
    data->push_back(p);
}

// begin: get the iterator at the front of this set of permutations.
Perms::iterator Perms::begin() {
    return this->data->begin();
}

// contains: true if and only if this contains specified permutation.
bool Perms::contains(Perm& p) {
    return find(this->begin(), this->end(), p) != this->end();
}

// end: get the iterator one past the last element of this set of permutations.
Perms::iterator Perms::end() {
    return this->data->end();
}

// filter: filter for only permutations with n matches in common with p.
void Perms::filter(Perm& p, int n) {
    vector<Perm>* newData = new vector<Perm>();
    for (Perms::iterator it = this->begin(); it != this->end(); ++it) {
        if (numInCommon(p, *it) == n) {
            newData->push_back(*it);
        }
    }

    delete this->data;
    this->data = newData;
}

void Perms::filter(Match& m, bool isMatch) {
    vector<Perm>* newData = new vector<Perm>();
    for (Perms::iterator it = this->begin(); it != this->end(); ++it) {
        if (isMatch == ((*it)[m.index] == m.charAtIndex)) {
            newData->push_back(*it);
        }
    }

    delete this->data;
    this->data = newData;
}

Perm& Perms::get(int i) {
    return this->data->at(i);
}

// populateAll(int): Fills with all permutations of the provided length.
void Perms::populateAll() {
    Perm p = "0123456789";
    do {
        this->add(p);
    } while (next_permutation(p.begin(), p.end()));
}

// populateFromFile(string): Fills with all permutations in the file at provided path.
void Perms::populateFromFile(string filename) {
    Perm s;
    ifstream inputFile(filename);

    while (inputFile >> s) {
        this->add(s);
    }

    inputFile.close();
}

// sample: peek at some element in the permutation set.
Perm& Perms::sample() {
    return this->data->front();
}

// size: get the number of elements in the permutation set.
vector<Perm>::size_type Perms::size() {
    return this->data->size();
}

// copyIntoChunks: copy into numChunks evenly sized chunks.
// User is responsible for freeing returned chunks.
Perms** Perms::copyIntoChunks(int numChunks) {
    Perms** chunks = new Perms*[numChunks];
    int chunkSize = this->size() / numChunks;
    int numChunksWithOneExtra = this->size() % numChunks;

    int i = 0;
    for (Perms::iterator beginChunk = this->begin(), endChunk = this->begin();
         beginChunk != this->end();
         beginChunk = endChunk) {
        endChunk += chunkSize;
        if (numChunksWithOneExtra-- > 0) {
            ++endChunk;
        }

        chunks[i++] = new Perms(beginChunk, endChunk);
    }
    assert(i == numChunks);

    return chunks;
}
