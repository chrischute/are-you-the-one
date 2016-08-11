/**
 * PerfectMatching.cpp
 * Christopher Chute
 *
 * Typedef and utilities for working with a single permutation.
 */

#include <string>
#include "PerfectMatching.h"

int numInCommon(PerfectMatching const& pm1, PerfectMatching const& pm2) {
    int numInCommon = 0;
    for (unsigned int i = 0; i < pm1.size() && i < pm2.size(); ++i)
        if (pm1[i] == pm2[i])
            ++numInCommon;
    return numInCommon;
}
