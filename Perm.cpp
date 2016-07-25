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
    } while (next_permutation(perm.begin(), perm.end()));

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

    for (Perms_citer it = before->begin(); it != before->end(); ++it) {
        if (Perm_distance(*it, against) == n) {
            after->push_back(*it);
        }
    }

    return after;
}


Perms Perms_init_from_file(std::string path)
{
    Perms perms = Perms_init_empty();
    Perm line;
    ifstream fin(path);

    while (fin >> line) {
        Perms_add(perms, line);
    }

    fin.close();
    return perms;
}

bool is_Perm_in_Perms(const Perm needle, const Perms haystack)
{
    return find(haystack->begin(), haystack->end(), needle) !=
           haystack->end();
}

Perm Perms_get(const Perms p, const int i)
{
    int sz = p->size();
    if (i < 0 || i >= sz) {
        return "";
    }
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

Perm Perm_from_chars(const char arr[], int len)
{
    return string(arr, len);
}

string Perm_tostring(const Perm p)
{
    stringstream builder;

    builder << "(";
    for (Perm_citer i = p.begin(); i != p.end(); i++) {
        builder << *i;
        if (i != p.end() - 1)
            builder << ", ";
    }
    builder << ")";

    return builder.str();
}

Perm Perm_fromstring(const string s)
{
    return (Perm) s;
}
