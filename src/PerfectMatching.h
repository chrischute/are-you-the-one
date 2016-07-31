/**
 * PerfectMatching.h
 * Christopher Chute
 *
 * Typedef and utilities for working with a single permutation.
 */

#ifndef GUARD_PERFECT_MATCHING_H
#define GUARD_PERFECT_MATCHING_H

// PerfectMatching: Perfect matching represented as permutation of "0123456789".
typedef std::string PerfectMatching;

// numInCommon(Pm const&, Pm const&): Number of positions where pm1 is same as pm2.
int numInCommon(PerfectMatching const& pm1, PerfectMatching const& pm2);

#endif
