/**
 * Perms.cpp
 * Christopher Chute
 * 
 * Data structure and operations for permutations.
 */

#include "Perms.h"
using namespace std;

// Perms(): Initialize empty set of permutations.
Perms::Perms() {
    data = new vector<Perm>();
}

// Perms(Perms&): Copy from another set of permutations.
Perms::Perms(const Perms& that) {
    data = new vector<Perm>();
    for (iterator it = that.begin(); it != that.end(); ++it) {
        data->push_back(*it);
    }
}

// ~Perms: Destroy all associated data.
Perms::~Perms() {
    delete data;
}

// add: add an element to the set of permutations.
void Perms::add(Perm& p) {
    this.data->push_back(p);
}

// begin: get the iterator at the front of this set of permutations.
iterator Perms::begin() {
    return this.data->begin();
}
// contains: true if and only if this contains specified permutation.
bool Perms::contains(Perm& p) {
    return find(this.begin(), this.end(), p) != this.end();
}

// end: get the iterator one past the last element of this set of permutations.
iterator Perms::end() {
    return this.data->end();
}

// numInCommon(Perm, Perm): Number of positions where p1 is same as p2.
int numInCommon(const Perm& p1, const Perm& p2) {
    int l1 = p1.size();
    int l2 = p2.size();
    int score = 0;

    for (int i = 0; i < l1 && i < l2; ++i) {
        if (p1[i] == p2[i]) {
            ++score;
        }
    }

    return score;
}

// filter: filter for only permutations with n matches in common with p.
void Perms::filter(Perm& p, int n) {
    vector<Perm>* newData = new vector<Perm>();
    for (iterator it = this.begin(); it != this.end(); ++it) {
        if (numInCommon(p, *it) == n) {
            newData->push_back(*it);
        }
    }

    delete this.data;
    this.data = newData;
}

void Perms::filter(Match& m, bool isMatch) {
    vector<Perm>* newData = new vector<Perm>();
    for (iterator it = this.begin(); it != this.end(); ++it) {
        if (isMatch == ((*it)[m.index] == m.charAtIndex)) {
            newData->push_back(*it);
        }
    }

    delete this.data;
    this.data = newData;
}

// populateAll(int): Fills with all permutations of the provided length.
void Perms::populateAll() {
    Perm p = new Perm("0123456789");
    do {
        this.add(p);
    } while (next_permutation(p.begin(), p.end()));
}

// populateFromFile(ifstream): Fills with all permutations in the file at provided path.
void Perms::populateFromFile(Perm filename) {
    Perm s;
    ifstream inputFile(filename);

    while (inputFile >> s) {
        this.add(s);
    }
}

// sample: peek at some element in the permutation set.
Perm& Perms::sample() {
    if (this.data->empty()) {
        return "";
    } else {
        return this.data->front();
    }
}

// size: get the number of elements in the permutation set.
vector<Perm>::size_type Perms::size() {
    return this.data->size();
}
