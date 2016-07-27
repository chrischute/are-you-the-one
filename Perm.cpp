/**
 * Perm.cpp
 * Christopher Chute
 *
 * Typedef and utilities for working with a single permutation.
 */

#include "Perm.h"
using namespace std;

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

string getPrintablePerm(const Perm& p) {
    stringstream builder;

    builder << "(";
    for (Perm_citer i = p.begin(); i != p.end(); i++) {
        builder << *i;
        if (i != p.end() - 1)
            builder << ", ";
    }
    builder << ")";

    return builder.str();
}
