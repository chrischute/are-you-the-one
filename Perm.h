/**
 * Perm.h
 * Christopher Chute
 * 
 * Declarations for permutations.
 */

#ifndef GUARD_PERM_H
#define GUARD_PERM_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

typedef std::string Perm;          // Represent permutations with strings.
typedef std::vector<Perm>* Perms;  // Group permutations in vector<Perm>'s.

Perms Perms_init_empty(void);
Perms Perms_init_full(void);
void Perms_destroy(Perms p);
void Perms_add(Perms p, const Perm to_add);
Perm Perms_get(const Perms p, const int i);
Perms Perms_filter(const Perms p, const Perm against, const int n);
int Perms_size(const Perms p);

int Perm_distance(const Perm& p, const Perm& q);
int Perm_length(const Perm& p);
std::string Perm_tostring(const Perm p);

#endif