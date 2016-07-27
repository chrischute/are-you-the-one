/**
 * Perm.h
 * Christopher Chute
 *
 * Typedef and utilities for working with a single permutation.
 */

#ifndef GUARD_PERM_H
#define GUARD_PERM_H

#include <sstream>
#include <string>

// Perm: Permutation represented as a string permutation of 0123456789.
typedef std::string Perm;

// numInCommon(Perm, Perm): Number of positions where p1 is same as p2.
int numInCommon(const Perm& p1, const Perm& p2);
// getPrintable(const Perm& p): get a nicely printable sequence rep.
std::string getPrintablePerm(const Perm& p);

#endif
