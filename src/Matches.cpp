/**
 * Matches.cpp
 * Christopher Chute
 *
 * Data structure and operations for single matches.
 */

#include "Matches.h"
using namespace std;

Matches::Matches() {
    this->data = new vector<Match>();
}

Matches::~Matches() {
    delete this->data;
}

void Matches::add(Match& m) {
    this->data->push_back(m);
}

bool Matches::contains(const Match& m) {
    return find(this->data->begin(), this->data->end(), m)
           != this->data->end();
}

Match& Matches::get(int i) {
    return this->data->at(i);
}

vector<Match>::size_type Matches::size() {
    return this->data->size();
}
