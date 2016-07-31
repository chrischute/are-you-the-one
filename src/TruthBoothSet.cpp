/**
 * TruthBoothSet.cpp
 * Christopher Chute
 *
 * Data structure and utilities for container of Truth Booth guesses.
 */

#include <algorithm>
#include "TruthBoothSet.h"

using std::find;
using std::vector;

typedef TruthBooth Tb;

TruthBoothSet::TruthBoothSet() {
    this->data = new vector<Tb>();
}

TruthBoothSet::~TruthBoothSet() {
    delete this->data;
}

void TruthBoothSet::add(Tb& m) {
    this->data->push_back(m);
}

bool TruthBoothSet::contains(Tb const& m) {
    return find(this->data->begin(), this->data->end(), m)
           != this->data->end();
}

Tb& TruthBoothSet::get(int i) {
    return this->data->at(i);
}

vector<Tb>::size_type TruthBoothSet::size() {
    return this->data->size();
}
