/**
 * PerfectMatching.cpp
 * Christopher Chute
 *
 * Typedef and utilities for working with a single permutation.
 */

#include <sstream>
#include "PerfectMatching.h"

using std::string;
using std::stringstream;

int numInCommon(PerfectMatching const& p1, PerfectMatching const& p2) {
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

string getPrintablePerm(PerfectMatching const& p) {
    stringstream printableStringBuilder;

    printableStringBuilder << "(";
    for (PerfectMatching::const_iterator i = p.begin(); i != p.end(); i++) {
        printableStringBuilder << *i;
        if (i != p.end() - 1)
            printableStringBuilder << ", ";
    }
    printableStringBuilder << ")";

    return printableStringBuilder.str();
}
