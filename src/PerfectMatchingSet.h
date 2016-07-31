/**
 * PerfectMatchingSet.h
 * Christopher Chute
 * 
 * Data structure and operations for permutations.
 */

#ifndef GUARD_PERFECT_MATCHING_SET_H
#define GUARD_PERFECT_MATCHING_SET_H

#include <vector>
#include "PerfectMatching.h"
#include "TruthBooth.h"

class PerfectMatchingSet {
private:
    std::vector<PerfectMatching>* data;
public:
    typedef std::vector<PerfectMatching>::iterator iterator;
    typedef std::vector<PerfectMatching>::const_iterator const_iterator;

    PerfectMatchingSet();
    PerfectMatchingSet(iterator begin, iterator end);
    ~PerfectMatchingSet();

    /**
     * Add an element to the container.
     * @param pm Element to add to this PerfectMatchingSet.
     */
    void add(PerfectMatching& pm);

    /**
     * Get the begin iterator, pointing to the first element of the container.
     * @return Beginning iterator for container.
     */
    iterator begin();

    /**
     * Check whether this PerfectMatchingSet contains a specific PerfectMatching.
     * @param pm The perfect matching to check for.
     * @return True if pm is contained in this PerfectMatchingSet, false otherwise.
     */
    bool contains(PerfectMatching const& pm);

    /**
     * Get the end iterator, one past last element.
     * @return Terminating iterator for container.
     */
    iterator end();

    /**
     * Remove all PerfectMatchings which are no longer possible after PM guess
     * @param pmGuess The PerfectMatching guess that was submitted.
     * @param numCorrect The feedback on pmGuess, i.e., how many were correct.
     */
    void filter(PerfectMatching const& pmGuess, int numCorrect);

    /**
     * Remove all PerfectMatchings which are no longer possible after TB guess.
     * @param tbGuess The TruthBooth guess that was submitted.
     * @param isMatch The feedback on tbGuess, i.e., whether it was a match.
     */
    void filter(TruthBooth const& tbGuess, bool isMatch);

    /**
     * Get the element at index i, not checking for data bounds.
     * @param i Index of element to get.
     * @return Element in the i-th position.
     */
    PerfectMatching& get(int i);

    /**
     * Fill with all permutations of the digits 0 through 9.
     */
    void populateAll();

    /**
     * Fill with all permutations in the file at specified path.
     * @param filename
     * Format: File should contain a single 10-digit number per line, nothing else.
     */
    void populateFromFile(std::string);

    /**
     * Get the number of elements in the permutation set.
     * @return Number of elements in container.
     */
    std::vector<PerfectMatching>::size_type size();

    /**
     * Copy container into array of sets with numChunks evenly sized chunks.
     * @param numChunks Number of chunks in the returned array.
     * @return Array of pointers to PerfectMatchingSets, each a partition of this set.
     */
    PerfectMatchingSet** copyIntoChunks(int numChunks);
};

#endif
