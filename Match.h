/**
 * Match.h
 * Christopher Chute
 *
 * Data structure for a single match.
 */

#ifndef GUARD_MATCH_H
#define GUARD_MATCH_H
class Match {
public:
    int index;
    char charAtIndex;
    Match() : index(0), charAtIndex('0') { }
    Match(int i, char c) : index(i), charAtIndex(c) { }
};
#endif
