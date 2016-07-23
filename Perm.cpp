/**
 * Perm.cpp
 * Christopher Chute
 * 
 * Data structure and operations for permutations.
 */

#include "Perm.h"

using namespace std;

Perms Perms_init_full(void)
{
    Perms perms = new vector<string>();
    
    Perm perm = "0123456789";
    do {
        perms->push_back(perm);
    } while (next_permutation(perms->begin(), perms->end()));

    return perms;
}

Perms Perms_init_empty(void)
{
    return new vector<string>();
}

void Perms_add(Perms p, const Perm to_add)
{
    p->push_back(to_add);
}

Perms Perms_filter(const Perms before, const Perm against, const int n)
{
    Perms after = Perms_init_empty();

    for (vector<string>::const_iterator it = before->begin();
        it != before->end();
        ++it) {
        if (Perm_distance(*it, against) == n) {
            after->push_back(*it);
        }
    }

    return after;
}

Perm Perms_get(const Perms p, const int i)
{
    return (*p)[i];
}

int Perms_size(const Perms p)
{
    return p->size();
}

void Perms_destroy(Perms p)
{
    if (p != NULL)
        delete p;
}

int Perm_distance(const Perm& p, const Perm& q)
{
    int n_matching = 0;
    int p_len = p.size();
    int q_len = q.size();

    for (int i = 0; i < q_len && i < p_len; ++i) {
        if (p[i] == q[i]) {
            ++n_matching;
        }
    }

    return n_matching;
}

int Perm_length(const Perm& p)
{
    return p.size();
}

string Perm_tostring(const Perm p)
{
    stringstream builder;

    builder << "(";
    for (Perm::const_iterator i = p.begin(); i != p.end(); i++) {
        builder << *i;
        if (i != p.end() - 1)
            builder << ", ";
    }
    builder << ")";

    return builder.str();
}
