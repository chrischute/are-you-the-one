/**
 * PerfectMatching.h
 * Christopher Chute
 *
 * Typedef and utilities for working with a single permutation.
 */

#ifndef GUARD_PERFECT_MATCHING_H
#define GUARD_PERFECT_MATCHING_H

#include <string>

// PerfectMatching: Perfect matching represented as permutation of "0123456789".
typedef std::string PerfectMatching;

// numInCommon(Pm const&, Pm const&): Number of positions where p1 is same as p2.
int numInCommon(PerfectMatching const&, PerfectMatching const&);

// getPrintable(const Pm& p): Get a nicely printable string from a PerfectMatching.
std::string getPrintablePerm(PerfectMatching const&);

#endif
