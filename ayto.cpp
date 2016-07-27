/**
 * ayto.cpp
 * Christopher Chute
 * 
 * Simulates minimax query algorithm for MTV's Are You The One?
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <mutex>
#include <cstring>
#include <thread>
#include <vector>
#include "Perm.h"
#include "Perms.h"
#include "Match.h"
#include "Matches.h"

#define NUM_CHUNKS (8)
#define NUM_PAIRS (10)
#define NUM_PAIRS_SQUARED (100)
#define PARTIAL_MINIMAX_THRESHOLD (10000) // Start partial minimax when <= this # left.
#define FULL_MINIMAX_THRESHOLD (314) // Only do full minimax when <= this # left.
#define QUERY_POOL ("test/query_pool.txt")
#define DIGITS ("0123456789")
#define RAND_ANSWER ("9352741680")
#define G1 ("0123456789")
#define G2 ("4579108623")

using namespace std;

typedef struct args Thread_args;
struct args {
    int _id;                       // ID of thread taking these args.
    Perms* _poss_answers;          // All permutations still possible to be the answer.
    Perms* _poss_queries;          // The chunk of potential queries to evaluate.
    Perms* _queries_made;          // All queries made so far.
    map<Perm, int>* _best_queries; // Where each thread will store it's best query.
    mutex* _mutex;                 // Mutex for the shared _best_queries map.
};

inline int sq(int x) { return x * x; }

// minimax_per_thread: run minimax on all permutations with id as first digit.
void minimax_per_thread(Thread_args *args)
{
    int thread_id = args->_id;
    Perms *poss_answers = args->_poss_answers;
    Perms *poss_queries = args->_poss_queries;
    Perms *queries_made = args->_queries_made;
    map<Perm, int> *best_queries = args->_best_queries; // Place best query from chunk here.
    mutex *write_lock = args->_mutex;
    bool do_full_minimax = poss_queries == NULL;

    // Find the query in poss_queries which eliminates the most possible answers.
    Perm best_query = DIGITS;
    int best_worst_case = poss_answers->size();

    if (do_full_minimax) {
        const char fd[] = { static_cast<char>('0' + thread_id) };
        const string first_digit = string(fd, 1); // stays fixed as we loop over permutations of last_nine_digits
        const string digits = DIGITS;
        string last_nine_digits = digits.substr(0, thread_id) + digits.substr(thread_id + 1);

        do {
            Perm query = first_digit + last_nine_digits;
            if (!queries_made->contains(query)) {
                vector<int> match_count(NUM_PAIRS + 1, 0); // Maps #hits to count
                // Check how many possibilities would remain after guessing query
                for (Perms::const_iterator it = poss_answers->begin();
                    it != poss_answers->end();
                    ++it) {
                    match_count[numInCommon(query, *it)] += 1;
                }
                // Find the # remaining in the worst-case scenario.
                int worst_case =
                        *max_element(match_count.begin(), match_count.end());
                // If this is lowest worst-case #remaining, set low water mark.
                if (worst_case < best_worst_case) {
                    best_query = query;
                    best_worst_case = worst_case;
                }
            }
        } while (next_permutation(last_nine_digits.begin(), last_nine_digits.end()));
    } else {
        for (Perms::const_iterator poss_query = poss_queries->begin();
             poss_query != poss_queries->end();
             ++poss_query) {
            Perm query = *poss_query;

            if (!queries_made->contains(query)) {
                vector<int> match_count(NUM_PAIRS + 1, 0); // Maps #hits to count
                // Check how many possibilities would remain after guessing query
                for (Perms::const_iterator it = poss_answers->begin();
                     it != poss_answers->end();
                     ++it) {
                    match_count[numInCommon(query, *it)] += 1;
                }
                // Find the # remaining in the worst-case scenario.
                int worst_case =
                        *max_element(match_count.begin(), match_count.end());
                // If this is lowest worst-case #remaining, set low water mark.
                if (worst_case < best_worst_case) {
                    best_query = query;
                    best_worst_case = worst_case;
                }
            }
        }
    }

    write_lock->lock();
    best_queries->insert(pair<Perm, int>(best_query, best_worst_case));
    write_lock->unlock();

    return;
}

Perm minimax(Perms* poss, Perms* queries_made)
{
    Perm best_query = "";
    int best_worst_case = poss->size();

    // Spin up 10 threads to run minimax in parallel.
    map<Perm, int> *best_queries = new map<Perm, int>();
    mutex *best_queries_lock = new mutex();
    vector<thread> thread_jobs;
    vector<Thread_args *> thread_args;
    Perms* chunks[NUM_PAIRS] = { NULL };

    if (poss->size() > PARTIAL_MINIMAX_THRESHOLD) {
        Perms* pool = new Perms();
        pool->populateFromFile(QUERY_POOL);
        cout << "Choosing best from pool of " << pool->size()
             << " at " << QUERY_POOL << endl;

        // Divide the possible queries into 10 chunks to evaluate.
        int chunk_size = pool->size() / NUM_PAIRS;
        int chunks_with_extra = pool->size() % NUM_PAIRS;
        for (int i = 0; i < NUM_PAIRS; ++i) {
            chunks[i] = new Perms();
            int n_to_add = chunks_with_extra-- > 0 ? chunk_size + 1 : chunk_size;
            Perms::iterator begin_chunk = pool->begin();
            Perms::iterator end_chunk = begin_chunk + n_to_add;

            Perms* chunk = new Perms(begin_chunk, end_chunk);
            chunks[i] = chunk;
        }

        for (int id = 0; id < NUM_PAIRS; ++id) {
            Thread_args *args = new Thread_args {
                    ._id = id,
                    ._poss_answers = poss,
                    ._poss_queries = chunks[id],
                    ._queries_made = queries_made,
                    ._best_queries = best_queries,
                    ._mutex = best_queries_lock
            };
            thread_args.push_back(args);
            thread_jobs.push_back(thread(minimax_per_thread, args));
        }
    } else if (poss->size() > FULL_MINIMAX_THRESHOLD) {
        cout << "Partial minimax with " << poss->size()
             << " answers remaining." << endl;
        // Divide the possible queries into 10 chunks to evaluate.
        int chunk_size = poss->size() / NUM_PAIRS;
        int chunks_with_extra = poss->size() % NUM_PAIRS;
        for (int i = 0; i < NUM_PAIRS; ++i) {
            int n_to_add = chunks_with_extra-- > 0 ? chunk_size + 1 : chunk_size;
            Perms::iterator begin_chunk = poss->begin();
            Perms::iterator end_chunk = begin_chunk + n_to_add;

            chunks[i] = new Perms(begin_chunk, end_chunk);
        }

        for (int id = 0; id < NUM_PAIRS; ++id) {
            Thread_args *args = new Thread_args {
                    ._id = id,
                    ._poss_answers = poss,
                    ._poss_queries = chunks[id],
                    ._queries_made = queries_made,
                    ._best_queries = best_queries,
                    ._mutex = best_queries_lock
            };
            thread_args.push_back(args);
            thread_jobs.push_back(thread(minimax_per_thread, args));
        }
    } else {
        cout << "Full minimax with " << poss->size()
             << " answers remaining." << endl;
        for (int id = 0; id < NUM_PAIRS; ++id) {
            Thread_args *args = new Thread_args {
                    ._id = id,
                    ._poss_answers = poss,
                    ._poss_queries = NULL,
                    ._queries_made = queries_made,
                    ._best_queries = best_queries,
                    ._mutex = best_queries_lock
            };
            thread_args.push_back(args);
            thread_jobs.push_back(thread(minimax_per_thread, args));
        }
    }
    // Wait for all threads to finish up.
    for (vector<thread>::iterator it = thread_jobs.begin();
            it != thread_jobs.end();
            ++it) {
        it->join();
    }

    // Find the best worst-case #remaining in candidates.
    for (map<Perm, int>::const_iterator it = best_queries->begin();
            it != best_queries->end();
            ++it) {
        Perm query = it->first;
        int worst_case_num_remaining = it->second;

        if (worst_case_num_remaining < best_worst_case) {
            best_worst_case = worst_case_num_remaining;
            best_query = query;
        }
    }

    // Clean up allocated args, chunks, map, and mutex.
    for (vector<Thread_args *>::iterator it = thread_args.begin();
            it != thread_args.end();
            ++it) {
        delete *it;
    }
    for (int i = 0; i < NUM_PAIRS; ++i) {
        if (chunks[i] != NULL) {
            delete chunks[i];
        }
    }
    delete best_queries;
    delete best_queries_lock;

    return best_query;
}

// Converts 2-digit index to permutation.
Perm i_2digit_to_Perm(int i)
{
    const char p[] = {
            static_cast<char>('0' + (i / 10)),
            static_cast<char>('0' + (i % 10))
    };

    return string(p, 2);
}

Match get_truthbooth(Perms* poss, Matches* queries_made)
{
    int *pair_count = new int[NUM_PAIRS_SQUARED]();
    Match best_pair(0);

    Match closest_match(-1, '-');
    if (queries_made->size() > 0) {
        for (Perms::const_iterator it = poss->begin(); it != poss->end(); ++it) {
            for (int i = 0; i < NUM_PAIRS; ++i) {
                int digit = (*it)[i] - '0';
                pair_count[NUM_PAIRS * i + digit] += 1;
            }
        }

        // Ideally a pairing occurs in 1/2 of all possible answers
        int ideal_count = poss->size() / 2;
        int closest_dist = poss->size() + 1;
        for (int i = 0; i < NUM_PAIRS_SQUARED; ++i) {
            Match couple(i);
            if (!queries_made->contains(couple) &&
                    abs(pair_count[i] - ideal_count) < closest_dist) {
                closest_dist = abs(pair_count[i] - ideal_count);
                closest_match = couple;
            }
        }
        assert(closest_match.index >= 0);
        best_pair = closest_match;
    }

    delete[] pair_count;
    return best_pair;
}

Perm get_fullpairing(Perms* poss, Perms* queries_made)
{
    if (poss->size() == 1) {
        return poss->sample();
    }
    Perm full_to_guess = DIGITS;
    switch (queries_made->size()) {
        case 0:
            full_to_guess = DIGITS;
            break;
        case 1:
            full_to_guess = G2;
            break;
        default:
            full_to_guess = minimax(poss, queries_made);
            break;
    }

    return full_to_guess;
}

void simulate_ayto_season(Perm answer)
{
    Perms* poss = new Perms(); // Remaining possibilities for answer.
    Matches* tb_queries = new Matches(); // Queries submitted in truth booth.
    Perms* pm_queries = new Perms(); // Queries submitted in perf. match.

    cout << "Answer " << answer << endl;
    poss->populateAll();
    while (true) {
        cout << "Round " << (tb_queries->size() + 1) << endl;
        //cout << "Truth booth... ";
        Match pair_to_guess = get_truthbooth(poss, tb_queries);
        //cout << pair_to_guess << endl;
        tb_queries->add(pair_to_guess);
        int isPairCorrect = pair_to_guess.isIn(answer);
        poss->filter(pair_to_guess, isPairCorrect);

        //cout << "Full pairing... " << flush;
        Perm full_to_guess = get_fullpairing(poss, pm_queries);
        //cout << full_to_guess << endl;
        pm_queries->add(full_to_guess);
        int n_correct = numInCommon(full_to_guess, answer);
        if (n_correct == answer.size()) {
            break;
        }
        poss->filter(full_to_guess, n_correct);

        cout << "Down to " << poss->size() << " remaining." << endl;
    }

    cout << "Results:" << endl;
    for (int i = 0; i < tb_queries->size(); ++i) {
        cout << "[Round " << (i + 1) << "] "
             << getPrintablePerm(tb_queries->get(i).toPerm()) << ", "
             << getPrintablePerm(pm_queries->get(i)) << "." << endl;
    }

    delete poss;
}

void interactive_ayto_season()
{
    Perms* poss = new Perms(); // Remaining possibilities for answer.
    Matches* tb_queries = new Matches(); // Queries submitted in truth booth.
    Perms* pm_queries = new Perms(); // Queries submitted in perf. match.


    cout << "Interactive Mode" << endl;
    poss->populateAll();
    while (true) {
        cout << "Round " << (tb_queries->size() + 1) << endl;

        Match pair_to_guess = get_truthbooth(poss, tb_queries);
        cout << "Truth booth: Is there a '" << pair_to_guess.charAtIndex << "' in position "
             << pair_to_guess.index << " (indexed starting with 0)? [yes/no]" << endl;
        tb_queries->add(pair_to_guess);
        string user_response;
        cin >> user_response;
        if (user_response[0] == 'y' || user_response[0] == 'Y') {
            poss->filter(pair_to_guess, 1);
        } else {
            poss->filter(pair_to_guess, 0);
        }

        Perm full_to_guess = get_fullpairing(poss, pm_queries);
        cout << "Perfect matching: How many of the following are in the correct spot?" << endl;
        cout << getPrintablePerm(full_to_guess) << endl;
        int n_correct;
        cin >> n_correct;
        pm_queries->add(full_to_guess);
        if (n_correct == NUM_PAIRS) {
            break;
        }
        poss->filter(full_to_guess, n_correct);

        if (poss->size() == 0) {
            cout << "Error: That combination of responses is not possible." << endl
                 << "Please check your responses and try again." << endl;
            return;
        }
        cout << "Down to " << poss->size() << " remaining." << endl;
    }

    cout << "Results:" << endl;
    for (int i = 0; i < tb_queries->size(); ++i) {
        cout << "[Round " << (i + 1) << "] "
             << getPrintablePerm((tb_queries->get(i).toPerm())) << ", "
             << getPrintablePerm((pm_queries->get(i))) << "." << endl;
    }

    delete poss;
}

bool is_runall(int argc, char **argv)
{
    return argc == 2 &&
        (strncmp(argv[1], "-all", 4) == 0
            || strncmp(argv[1], "-a", 2) == 0);
}

bool is_runfile(int argc, char **argv)
{
    return argc == 3 &&
            (strncmp(argv[1], "-file", 5) == 0
                || strncmp(argv[1], "-f", 2) == 0);

}

bool is_interactive_mode(int argc, char **argv) {
    return argc == 2 && strncmp(argv[1], "-i", 2) == 0;
}

void usage(void)
{
    cout << "usage: ./ayto" << endl;
    cout << "\t[-a | -all]                - Run on all permutations" << endl;
    cout << "\t[[-f | -file] <file_name>] - Run on permutations in file" << endl;
    // TODO: cout << "\t[-i]                       - Interactive, wait for user_response" << endl;
}

int main(int argc, char **argv) {
    Perm answer = RAND_ANSWER;       // The hidden matching.

    if (is_runall(argc, argv)) {
        // Run on all 10! hidden matchings.
        answer = DIGITS;
        do {
            simulate_ayto_season(answer);
        } while (next_permutation(answer.begin(), answer.end()));
    } else if (is_runfile(argc, argv)) {
        Perms* answers = new Perms();
        answers->populateFromFile(argv[2]);
        for (Perms::const_iterator it = answers->begin();
                it != answers->end();
                ++it) {
            simulate_ayto_season(*it);
        }
    } else if (is_interactive_mode(argc, argv)) {
        interactive_ayto_season();
    } else if (argc == 1) {
        // Just run on a single hidden matching.
        simulate_ayto_season(answer);
    } else {
        usage();
        return 1;
    }

    return 0;
}
