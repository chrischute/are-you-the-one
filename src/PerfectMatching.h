/**
 * PerfectMatching.h
 * Christopher Chute
 *
 * Typedef and utilities for working with a single permutation.
 */

#ifndef GUARD_PERFECT_MATCHING_H
#define GUARD_PERFECT_MATCHING_H

typedef std::string PerfectMatching;

/**
 * Get the number of positions in which two PerfectMatchings share
 * the same character.
 * @param pm1 First PerfectMatching to compare.
 * @param pm2 Second PerfectMatching to compare.
 * @return Number of positions in which pm1 has same char as pm2.
 */
int numInCommon(PerfectMatching const& pm1, PerfectMatching const& pm2);

#endif
