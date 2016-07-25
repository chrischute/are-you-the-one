/**
 * Perm.h
 * Christopher Chute
 * 
 * Declarations for permutations.
 */

#ifndef GUARD_PERM_H
#define GUARD_PERM_H

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef std::string Perm;          // Represent permutations with strings.
typedef std::string::iterator Perm_iter;
typedef std::string::const_iterator Perm_citer;

typedef std::vector<Perm>* Perms;  // Group permutations in vector<Perm>'s.
typedef std::vector<Perm>::iterator Perms_iter;
typedef std::vector<Perm>::const_iterator Perms_citer;

Perms Perms_init_empty(void);
Perms Perms_init_full(void);
void Perms_destroy(Perms p);
void Perms_add(Perms p, const Perm to_add);
Perm Perms_get(const Perms p, const int i);
Perms Perms_filter(const Perms p, const Perm against, const int n);
bool is_Perm_in_Perms(const Perm needle, const Perms haystack);
int Perms_size(const Perms p);

int Perm_distance(const Perm& p, const Perm& q);
int Perm_length(const Perm& p);
Perm Perm_from_chars(const char arr[], int len);
std::string Perm_tostring(const Perm p);

#endif