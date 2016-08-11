/**
 * TruthBoothSet.h
 * Christopher Chute
 *
 * Data structure and utilities for container of Truth Booth guesses.
 */

#ifndef GUARD_TRUTH_BOOTH_SET_H
#define GUARD_TRUTH_BOOTH_SET_H

#include <vector>
#include "TruthBooth.h"

class TruthBoothSet {
private:
    std::vector<TruthBooth>* data;
public:
    TruthBoothSet();
    ~TruthBoothSet();

    /**
     * Add a TruthBooth element to this container.
     * @param truthBooth Element to add to this container.
     */
    void add(const TruthBooth& truthBooth);

    /**
     * Check whether this container contains a specific element.
     * @param truthBooth Element to search for in this container.
     * @return True if this container contains truthBooth, false otherwise.
     */
    bool contains(const TruthBooth& truthBooth) const;

    /**
     * Get the element at index i in this container.
     * @param i Index of element to fetch.
     * @return Element at index i.
     */
    TruthBooth& get(int i) const;

    /**
     * Get the number of elements in this container.
     * @return Number of elements in this container.
     */
    std::vector<TruthBooth>::size_type size() const;
};

#endif
