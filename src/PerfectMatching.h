/**
 * PerfectMatching.h
 * Christopher Chute
 *
 * Typedef and utilities for working with a single permutation.
 */

#ifndef GUARD_PERM_H
#define GUARD_PERM_H

#include <algorithm>
#include <string>

// PerfectMatching: Perfect matching represented as a string permutation of 0123456789.
typedef std::string PerfectMatching;

// numInCommon(PerfectMatching const&, PerfectMatching const&): Number of positions where p1 is same as p2.
int numInCommon(PerfectMatching const&, PerfectMatching const&);

// getPrintable(const PerfectMatching& p): get a nicely printable sequence rep.
std::string getPrintablePerm(PerfectMatching const&);

#endif
