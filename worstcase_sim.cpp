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

#define TEN_FACT (3628800)
#define NUM_CHUNKS (8)

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::map;
using std::pair;
using std::find;
using std::abs;
using std::atoi;
using std::next_permutation;
using std::max_element;
using std::accumulate;
using std::reverse;
using std::thread;

struct args {
	vector<string> *perms;
	vector<string> *chunk;
	pair<string, int> *cd;
	int thread_id;
};

void simulate_game(const string &hidden, map<string, int> &turns_taken,
				   bool running_all);
bool picmp(const pair<string, int> &p1, const pair<string, int> &p2);
double map_avg(const map<string, int> &mp);
int nrand(int n);
int evaluate(const string &sol, const string &query);
vector<string> remove_perms(vector<string> &perms, int eval, string &query);
pair<string, int> guess_tb(vector<string> &perms, vector<string> &guessed_tb, int turn);
pair<string, int> guess_pm(vector<string> &perms, vector<string> &guessed, int turn);
void make_chunks(vector<string> &orig, vector<vector<string> > &chunks, int n);
string min_candidate(pair<string, int> *candidates, int n);
void get_score(struct args *args);
int wc_response(string &guess, vector<string> &perms);
bool prcmp(pair<int, int> x, pair<int, int> y);
void sequence_print(string s);
void print_summary(map<string, int> &turns_taken);
struct args **create_args(vector<string> &perms, pair<string, int> *cd, vector<string> &chunk, int thread_id);

vector<string> ORIGPERMS;

int main(int argc, char **argv)
{
	int sz;
	map<string, int> turns_taken;
	vector<string> sample;
	const string digits = "0123456789";
	bool running_all = false;

	if (argc != 2 && argc != 3) {
		cout << "usage: 'ayto n [\"sample\"]'" << endl;
		return 1;
	} else {
		if (argc == 3) {
			if (strncmp(argv[2], "sample", 6) == 0) {
				string line;
				while (std::getline(cin, line))
					sample.push_back(line);
			}
		}
		if ((sz = atoi(argv[1])) < 0) {
			sz = -sz;
			running_all = true;
		}
		if (sz < 3 || sz > 10) {
			cout << "usage: 'ayto npairs' where 3 <= npairs <= 10" << endl;;
			return 1;
		}
	}

	// initialize ORIGPERMS and possible_perms
	string range = digits.substr(0, sz);
	do {
		ORIGPERMS.push_back(range);
	} while (next_permutation(range.begin(), range.end()));

	if (running_all) {
		for (vector<string>::const_iterator it = ORIGPERMS.begin();
			 it != ORIGPERMS.end(); ++it) {
			simulate_game(*it, turns_taken, running_all);
		}
		print_summary(turns_taken);
	} else if (!sample.empty()) {
		for (vector<string>::const_iterator it = sample.begin();
			 it != sample.end(); ++it) {
			simulate_game(*it, turns_taken, true);
		}
		print_summary(turns_taken);
	} else {
		string hidden = ORIGPERMS[nrand(ORIGPERMS.size())];
		simulate_game(hidden, turns_taken, running_all);
	}

	return 0;
}

// simulate_game:  run a single round of AYTO on hidden vector
void simulate_game(const string &hidden, map<string, int> &turns_taken,
				   bool running_all)
{
	vector<string> possible_perms = ORIGPERMS;
	pair<string, int> tbguess;
	pair<string, int> pmguess;
	vector<string> guessed;
	vector<string> guessed_tb;
	int e;
	int sz = hidden.size();

	if (!running_all) {
		cout << "Running AYTO Simulator on Hidden Vector: ";
		sequence_print(hidden);
		cout << endl;
	}

	for (int turn = 1; ; ++turn) {
		// stage one: truth booth
		if (!running_all) {
			cout << "**** Round " << turn << "A ****" << endl;
			cout << "Num. Possibilities: " << possible_perms.size() << endl;
		}
		tbguess = guess_tb(possible_perms, guessed_tb, turn);
		if (!running_all) {
			cout << "Guess: ";
			sequence_print(tbguess.first);
			cout << endl;
			e = tbguess.second;
			cout << "Worst-Case Evaluation: " << e << endl;
		} else {
			e = evaluate(hidden, tbguess.first);
		}
		possible_perms = remove_perms(possible_perms, e, tbguess.first);

		// stage two: perfect matching
		if (!running_all) {
			cout << "Round " << turn << "B" << endl;
			cout << "Num. Possibilities: " << possible_perms.size() << endl;
		}
		pmguess = guess_pm(possible_perms, guessed, turn);

		if (!running_all) {
			cout << "Guess: ";
			sequence_print(pmguess.first);
			cout << endl;
			e = pmguess.second;
			cout << "Worst-Case Evaluation: " << e << endl;
		} else {
			e = evaluate(hidden, pmguess.first);
		}
		if (e == sz) {
			cout << "Found ";
			sequence_print(pmguess.first);
			cout << " in " << turn << " guesses." << endl;
			turns_taken[pmguess.first] = turn;
			break;
		}

		possible_perms = remove_perms(possible_perms, e, pmguess.first);
	}
}

// map_avg:  returns average int component of a map<string, int> type
double map_avg(const map<string, int> &mp)
{
	double sum = 0.0;

	for (map<string, int>::const_iterator it = mp.begin(); 
		 it != mp.end(); ++it) {
		sum += it->second;
	}

	return sum / mp.size();
}

// picmp:  comparison function for pair<string, int> types, via int component
bool picmp(const pair<string, int> &p1, const pair<string, int> &p2)
{
	return p1.second < p2.second;
}

// nrand:  random integer in range [0, n)
int nrand(int n)
{
	srand(time(NULL));
	
	return rand() % n;
}

// evaluate:  number of black hits from permutation or truth booth query
int evaluate(const string &sol, const string &query)
{
	int hits = 0;

	if (sol.size() == query.size()) {
		// permutation query
		int s = sol.size();
		for (int i = 0; i < s; i++) {
			if (sol[i] == query[i])
				++hits;
		}
	} else {
		// truth booth query
		if (sol[atoi(query.substr(0, 1).c_str())] == query[1])
			++hits;
	}

	return hits;
}

// remove_perms:  remove solutions that are no longer possible after an eval
vector<string> remove_perms(vector<string> &perms, int eval, string &query)
{
	vector<string> new_perms;

	for (vector<string>::iterator i = perms.begin(); i != perms.end(); i++) {
		if (evaluate(*i, query) == eval) {
			new_perms.push_back(*i);
		}
	}

	return new_perms;
}

// guess_tb:  guesses best pair (pos, val) to go to the truth booth
pair<string, int> guess_tb(vector<string> &possible_perms,
						   vector<string> &guessed_tb, int turn)
{
	static const string digits = "0123456789";
	int n = possible_perms[0].size();
	pair<string, int> next_guess;

	if (turn == 1) {
		next_guess.first = "00";
		next_guess.second = 0;
	} else if (possible_perms.size() == 1) {
		next_guess.first = "0" + possible_perms[0].substr(0, 1);
		next_guess.second = 1;
	} else {
		map<string, double> pair_to_count;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				pair_to_count[digits.substr(i, 1) + digits.substr(j, 1)] = 0;
			}
		}

		// count up the occurrences of each pair in the possible perms
		for (vector<string>::iterator p = possible_perms.begin();
			 p != possible_perms.end(); p++) {
			int len = possible_perms[0].size();
			for (int i = 0; i < len; i++) {
				pair_to_count[digits.substr(i, 1) + (*p).substr(i, 1)] += 1;
			}
		}

		double best_dist = 1;
		int perm_cnt = possible_perms.size();
		for (map<string, double>::iterator i = pair_to_count.begin();
			 i != pair_to_count.end(); i++) {
			if (find(guessed_tb.begin(), guessed_tb.end(), i->first)
				== guessed_tb.end()) {
				// hasn't been guessed yet
				if (abs(i->second/perm_cnt - .5) < best_dist) {
					next_guess.first = i->first;
					best_dist = abs(i->second/perm_cnt - .5);
					if (i->second / perm_cnt < 0.5) // occurs in < half perms
						next_guess.second = 0;
					else                            // occurs in >= half perms
						next_guess.second = 1;
				}
			}
		}
	}

	guessed_tb.push_back(next_guess.first);

	return next_guess;
}

// guess_pm:  guess a full permutation using minimax
pair<string, int> guess_pm(vector<string> &possible_perms,
						   vector<string> &guessed, int turn)
{
	static const string digits = "0123456789";
	pair<string, int> next_guess;
	vector<vector<string> > chunks;
	int sz = possible_perms[0].size();

	// on first turn, we guess "0, 1, ..., n-1" if truth booth was correct
	// or "1, 0, ..., n-1" if truth booth was incorrect
	if (turn == 1) {
		int fact, i;
		for (i = 2, fact = 1; i <= sz; fact *= i++)
			;
		if (possible_perms.size() == fact) {
			next_guess.first = digits.substr(0, sz);
			next_guess.second = 1;
		} else {
			next_guess.first = "10" + digits.substr(2, sz - 2);
			next_guess.second = 1;
		}
	} else if (possible_perms.size() == 1) {
		next_guess.first = possible_perms[0];
		next_guess.second = possible_perms[0].size();
	} else {
		// run multi-threaded minimax to get next guess
		pair<string, int> candidates[NUM_CHUNKS];
		vector<thread> jobs;
		make_chunks(ORIGPERMS, chunks, NUM_CHUNKS);
		struct args **args = create_args(possible_perms, candidates, chunks[0], 0);

		for (int j = 0; j < NUM_CHUNKS; j++) {
			args[j]->chunk = &(chunks[j]);
			args[j]->thread_id = j;
			jobs.push_back(thread(get_score, args[j]));
		}
		for (int j = 0; j < NUM_CHUNKS; j++) {
			jobs[j].join();
		}
		
		next_guess.first = min_candidate(candidates, NUM_CHUNKS);
		next_guess.second = wc_response(next_guess.first, possible_perms);
		
		for (int j = 0; j < NUM_CHUNKS; j++)
			free(args[j]);
		free(args);
	}

	guessed.push_back(next_guess.first);

	return next_guess;
}

struct args **create_args(vector<string> &perms, pair<string, int> *cd, vector<string> &chunk, int thread_id)
{
	struct args **args = (struct args **) malloc(sizeof(struct args*)*NUM_CHUNKS);
	assert(args);
	for (int i = 0; i < NUM_CHUNKS; i++) {
		args[i] = (struct args *) malloc(sizeof(struct args));
		assert(args[i]);
		args[i]->perms = &perms;
		args[i]->cd = cd;
	}

	return args;
}

// make_chunks:  return pointers to n (nearly) equally sized vectors
//                from the original vector
void make_chunks(vector<string> &orig, vector<vector<string> > &chunks, int n)
{
    int sz = orig.size();
    int chunk_sz = sz / n;
    int n_with_extra = sz % n;
    vector<string>::iterator b = orig.begin();
    vector<string>::iterator e;

    for (int i = 0; i < n; i++) {
        int m = chunk_sz;    // size of this chunk
        if (n_with_extra) {
            ++m;
            --n_with_extra;
        }
        e = b + m;
        vector<string> subvec(b, e);
        chunks.push_back(subvec);
        b = e;
    }
}

// min_candidate:  string with min int from array of pair<string, ints>
string min_candidate(pair<string, int> *candidates, int n)
{
	int i, minsofar;
	string minstring;

	minstring = candidates[0].first;
	minsofar = candidates[0].second;
	for (i = 1; i < n; ++i) {
		if (candidates[i].second < minsofar) {
			minsofar = candidates[i].second;
			minstring = candidates[i].first;
		}
	}

	return minstring;
}

// get_score:  find the maximum number of remaining solutions over all
//             possible responses to the query s
//             this version takes a chunk and finds the guess with lowest score
//             from that chunk
void get_score(struct args *args)
{
	// parse the args struct
	vector<string> &chunk = *(args->chunk);
	vector<string> &perms = *(args->perms);
	pair<string, int> *cd = args->cd;
	int thread_id = args->thread_id;

	typedef vector<string>::const_iterator vec_iter;
	int sz = perms[0].size();

	pair<string, int> best_guess;
	best_guess.second = perms.size();
	int wc_num_remaining;
	for (vec_iter s = chunk.begin(); s != chunk.end(); ++s) {
		vector<int> matches(sz + 1, 0);
		for (vec_iter p = perms.begin(); p != perms.end(); ++p) {
			++matches[evaluate(*s, *p)];
		}
		wc_num_remaining = *max_element(matches.begin(), matches.end());
		if (wc_num_remaining < best_guess.second) {
			best_guess.first = *s;
			best_guess.second = wc_num_remaining;
		}
	}

	cd[thread_id] = best_guess;

	return;
}

// wc_response:  the response to guess that eliminates the least solutions
int wc_response(string &guess, vector<string> &perms)
{
	map<int, int> matches_eval;

	for (vector<string>::iterator it = perms.begin(); it!=perms.end(); ++it) {
		++matches_eval[evaluate(guess, *it)];
	}

	return max_element(matches_eval.begin(), matches_eval.end(), prcmp)->first;
}

// prcmp:  comparison function for pair<int, int> types in map
bool prcmp(pair<int, int> x, pair<int, int> y)
{
	return x.second < y.second;
}

// print_summary:  prints the analysis for multi-game simulation
void print_summary(map<string, int> &turns_taken)
{
	cout << "***** SUMMARY *****\n";
	cout << "Avg. Turns: " << map_avg(turns_taken) << endl;
	pair<string, int> wc = *max_element(turns_taken.begin(),
										turns_taken.end(), picmp);
	cout << "Worst Hidden Vector: ";
	sequence_print(wc.first);
	cout << " in " << wc.second << " turns." << endl;
}

void sequence_print(const string s)
{
	for (string::const_iterator i = s.begin(); i != s.end(); i++) {
		if (i == s.begin())
			cout << "(";
		cout << *i;
		if (i != s.end() - 1)
			cout << ", ";
		else
			cout << ")";
	}
}
