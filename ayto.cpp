/**
 * ayto.cpp
 * Christopher Chute
 * 
 * Simulates minimax query algorithm for MTV's Are You The One?
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <cmath>
#include "Perm.h"

#define TEN_FACT (3628800)
#define NUM_CHUNKS (8)
#define DIGITS ("0123456789")
#define DUMMY ("8476392105")

using namespace std;

Perm get_truthbooth(Perms poss, Perms queries_made)
{
	return "01";
}

Perm get_fullpairing(Perms poss, Perms queries_made)
{
	return "1234567890";
}

void simulate_ayto_season(Perm answer)
{
	Perms poss = Perms_init_full(); // Remaning possibilities for answer.
	Perms tb_queries = Perms_init_empty(); // Queries submitted in truth booth.
	Perms fp_queries = Perms_init_empty(); // Queries submitted in perf. match.

	cout << "Answer " << Perm_tostring(answer) << endl;
	while (true) {
		cout << "Round " << (Perms_size(tb_queries) + 1) << endl;
		cout << "Truth booth... ";
		Perm pair_to_guess = get_truthbooth(poss, tb_queries);
		cout << pair_to_guess << endl;
		Perms_add(tb_queries, pair_to_guess);
		int was_tb_correct = Perm_distance(pair_to_guess, answer);
		poss = Perms_filter(poss, pair_to_guess, was_tb_correct);

		cout << "Full pairing... ";
		Perm full_to_guess = get_fullpairing(poss, fp_queries);
		cout << full_to_guess << endl;
		Perms_add(fp_queries, full_to_guess);
		int num_fp_correct = Perm_distance(full_to_guess, answer);
		if (num_fp_correct == Perm_length(answer)) {
			break;
		}
		poss = Perms_filter(poss, full_to_guess, num_fp_correct);
	}

	cout << "Results:";
	for (int i = 0; i < Perms_size(tb_queries); ++i) {
		cout << "(" << i << ") "
			 << Perm_tostring(Perms_get(tb_queries, i)) << ", "
			 << Perm_tostring(Perms_get(fp_queries, i)) << "." << endl;
	}

	Perms_destroy(poss);
}

bool is_runall(int argc, char **argv)
{
	return argc == 2 &&
		(strncmp(argv[1], "-all", 4) == 0
			|| strncmp(argv[1], "-a", 2) == 0);
}

void usage(void)
{
	cout << "usage: 'ayto n [\"sample\"]'" << endl;
}

int main(int argc, char **argv)
{
	Perm answer = DUMMY;       // The hidden matching.

	if (is_runall(argc, argv)) {
		// Run on all 10! hidden matchings.
		answer = DIGITS;
		do {
			simulate_ayto_season(answer);
		} while (next_permutation(answer.begin(), answer.end()));
	} else if (argc == 1) {
		// Just run on a single hidden matching.
		simulate_ayto_season(answer);
	} else {
		usage();
		return 1;
	}

	return 0;
}
