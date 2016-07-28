/**
 * ayto.cpp
 * Christopher Chute
 * 
 * Simulates minimax query algorithm for MTV's Are You The One?
 */

#include "ayto.h"

int main(int argc, char **argv) {
    //bool isVerbose = true;      // TODO: Add -v flag for verbose setting.
    //bool isInteractive = false; // TODO: Clean up the switch for interactive vs. regular.

    if (isRunAllMode(argc, argv)) {
        // Run on all 10! possible answers (warning: will take ~9 years).
        Perm answer = DIGITS;
        do {
            runAyto(answer);
        } while (next_permutation(answer.begin(), answer.end()));
    } else if (isRunFileMode(argc, argv)) {
        // Run on all answers in specified file at argv[2].
        Perms* answers = new Perms();
        answers->populateFromFile(argv[2]);
        for (Perms::const_iterator it = answers->begin();
             it != answers->end();
             ++it) {
            runAyto(*it);
        }
    } else if (isRunInteractiveMode(argc, argv)) {
        // Let the user input feedback after each guess.
        runAytoInteractive();
    } else if (argc == 1) {
        // Just run on a single answer, the dummy answer.
        runAyto(DUMMY);
    } else {
        cout << "usage: ./ayto" << endl;
        cout << "\t[-a | -all]                - Run on all permutations" << endl;
        cout << "\t[[-f | -file] <file_name>] - Run on permutations in file" << endl;
        cout << "\t[-i]                       - Interactive, wait for user_response" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

Perm getNextPerfectMatchingGuess(Perms *poss, Perms *guessesAlreadyMade)
{
    Perm nextGuess;

    if (poss->size() == 1) {
        nextGuess = poss->get(0);
    } else {
        switch (guessesAlreadyMade->size()) {
            case 0:
                nextGuess = DIGITS;
                break;
            case 1:
                nextGuess = GUESS;
                break;
            default:
                nextGuess = minimax(poss, guessesAlreadyMade);
                break;
        }
    }

    return nextGuess;
}

Match getNextTruthBoothGuess(Perms *poss, Matches *guessesAlreadyMade)
{
    Match nextGuess(-1, '_');
    map<Match, int> numOccurrencesOfMatch;

    if (guessesAlreadyMade->size() == 0 ||
            poss->size() == 1) {
        return Match(0, '0');
    }

    // Count the number of occurrences in possible answers of each match.
    for (Perms::const_iterator iterPerm = poss->begin();
         iterPerm != poss->end();
         ++iterPerm) {
        Perm::const_iterator firstIterChar = iterPerm->begin();
        for (Perm::const_iterator iterChar = iterPerm->begin();
            iterChar != iterPerm->end();
            ++iterChar) {
            ++numOccurrencesOfMatch[Match(iterChar - firstIterChar, *iterChar)];
        }
    }

    // Ideally a pairing occurs in 1/2 of all possible answers.
    // The worst-case response is always the complement of the
    // best-case response. We want to optimize the worst case, which
    //  occurs when both yes/no cut the remaining solutions in half.
    int optimalNumOccurrences = poss->size() / 2;
    int closestNumOccurrences = poss->size() + 1;

    for (map<Match, int>::const_iterator it = numOccurrencesOfMatch.begin();
        it != numOccurrencesOfMatch.end();
        ++it) {
        if (abs(optimalNumOccurrences - it->second) <
                abs(optimalNumOccurrences - closestNumOccurrences)) {
            nextGuess = it->first;
            closestNumOccurrences = it->second;
        }
    }

    return nextGuess;
}

bool isRunAllMode(int argc, char **argv)
{
    return argc == 2 &&
           (strncmp(argv[1], "-all", 4) == 0
            || strncmp(argv[1], "-a", 2) == 0);
}

bool isRunFileMode(int argc, char **argv)
{
    return argc == 3 &&
           (strncmp(argv[1], "-file", 5) == 0
            || strncmp(argv[1], "-f", 2) == 0);

}

bool isRunInteractiveMode(int argc, char **argv) {
    return argc == 2 && strncmp(argv[1], "-i", 2) == 0;
}

Perm minimax(Perms* poss, Perms* queries_made)
{
    Perm best_query = "";
    int best_worst_case = poss->size();

    // Spin up 10 threads to run minimax in parallel.
    map<Perm, int> *best_queries = new map<Perm, int>();
    mutex *best_queries_lock = new mutex();
    vector<thread> thread_jobs;
    vector<struct targs*> thread_args;
    Perms* chunks[PERM_LENGTH] = { NULL };

    if (poss->size() > START_PART_MM) {
        Perms* pool = new Perms();
        pool->populateFromFile(POOL_FILENAME);
        cout << "Choosing best from pool of " << pool->size()
             << " at " << POOL_FILENAME << endl;

        // Divide the possible queries into 10 chunks to evaluate.
        int chunk_size = pool->size() / PERM_LENGTH;
        int chunks_with_extra = pool->size() % PERM_LENGTH;
        for (int i = 0; i < PERM_LENGTH; ++i) {
            chunks[i] = new Perms();
            int n_to_add = chunks_with_extra-- > 0 ? chunk_size + 1 : chunk_size;
            Perms::iterator begin_chunk = pool->begin();
            Perms::iterator end_chunk = begin_chunk + n_to_add;

            Perms* chunk = new Perms(begin_chunk, end_chunk);
            chunks[i] = chunk;
        }

        for (int id = 0; id < PERM_LENGTH; ++id) {
            Thread_args *args = new Thread_args {
                    ._id = id,
                    ._poss_answers = poss,
                    ._poss_queries = chunks[id],
                    ._queries_made = queries_made,
                    ._best_queries = best_queries,
                    ._mutex = best_queries_lock
            };
            thread_args.push_back(args);
            thread_jobs.push_back(thread(minimaxPerThread, args));
        }
    } else if (poss->size() > START_FULL_MM) {
        cout << "Partial minimax with " << poss->size()
             << " answers remaining." << endl;
        // Divide the possible queries into 10 chunks to evaluate.
        int chunk_size = poss->size() / PERM_LENGTH;
        int chunks_with_extra = poss->size() % PERM_LENGTH;
        for (int i = 0; i < PERM_LENGTH; ++i) {
            int n_to_add = chunks_with_extra-- > 0 ? chunk_size + 1 : chunk_size;
            Perms::iterator begin_chunk = poss->begin();
            Perms::iterator end_chunk = begin_chunk + n_to_add;

            chunks[i] = new Perms(begin_chunk, end_chunk);
        }

        for (int id = 0; id < PERM_LENGTH; ++id) {
            Thread_args *args = new Thread_args {
                    ._id = id,
                    ._poss_answers = poss,
                    ._poss_queries = chunks[id],
                    ._queries_made = queries_made,
                    ._best_queries = best_queries,
                    ._mutex = best_queries_lock
            };
            thread_args.push_back(args);
            thread_jobs.push_back(thread(minimaxPerThread, args));
        }
    } else {
        cout << "Full minimax with " << poss->size()
             << " answers remaining." << endl;
        for (int id = 0; id < PERM_LENGTH; ++id) {
            Thread_args *args = new Thread_args {
                    ._id = id,
                    ._poss_answers = poss,
                    ._poss_queries = NULL,
                    ._queries_made = queries_made,
                    ._best_queries = best_queries,
                    ._mutex = best_queries_lock
            };
            thread_args.push_back(args);
            thread_jobs.push_back(thread(minimaxPerThread, args));
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
    for (int i = 0; i < PERM_LENGTH; ++i) {
        if (chunks[i] != NULL) {
            delete chunks[i];
        }
    }
    delete best_queries;
    delete best_queries_lock;

    return best_query;
}

void minimaxPerThread(Thread_args *args)
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
                vector<int> match_count(PERM_LENGTH + 1, 0); // Maps #hits to count
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
                vector<int> match_count(PERM_LENGTH + 1, 0); // Maps #hits to count
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

void runAyto(Perm answer)
{
    Perms* poss = new Perms(); // Remaining possibilities for answer.
    Matches* tb_queries = new Matches(); // Queries submitted in truth booth.
    Perms* pm_queries = new Perms(); // Queries submitted in perf. match.

    cout << "Answer " << answer << endl;
    poss->populateAll();
    while (true) {
        cout << "Round " << (tb_queries->size() + 1) << endl;
        Match pair_to_guess = getNextTruthBoothGuess(poss, tb_queries);
        tb_queries->add(pair_to_guess);
        int isPairCorrect = pair_to_guess.isContainedIn(answer);
        poss->filter(pair_to_guess, isPairCorrect);

        Perm full_to_guess = getNextPerfectMatchingGuess(poss, pm_queries);
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
             << getPrintablePerm(tb_queries->get(i).toString()) << ", "
             << getPrintablePerm(pm_queries->get(i)) << "." << endl;
    }

    delete poss;
}

void runAytoInteractive()
{
    Perms* poss = new Perms(); // Remaining possibilities for answer.
    Matches* tb_queries = new Matches(); // Queries submitted in truth booth.
    Perms* pm_queries = new Perms(); // Queries submitted in perf. match.


    cout << "Interactive Mode" << endl;
    poss->populateAll();
    while (true) {
        cout << "Round " << (tb_queries->size() + 1) << endl;

        Match pair_to_guess = getNextTruthBoothGuess(poss, tb_queries);
        cout << "Truth booth: Is there a '" << pair_to_guess.charAtIndex
             << "' in position " << pair_to_guess.index
             << " (indexed starting with 0)? [yes/no]" << endl;
        tb_queries->add(pair_to_guess);
        string user_response;
        cin >> user_response;
        if (user_response[0] == 'y' || user_response[0] == 'Y') {
            poss->filter(pair_to_guess, 1);
        } else {
            poss->filter(pair_to_guess, 0);
        }

        Perm full_to_guess = getNextPerfectMatchingGuess(poss, pm_queries);
        cout << "Perfect matching: How many of the following "
             << "are in the correct spot?" << endl
             << getPrintablePerm(full_to_guess) << endl;
        int n_correct;
        cin >> n_correct;
        pm_queries->add(full_to_guess);
        if (n_correct == PERM_LENGTH) {
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
             << getPrintablePerm((tb_queries->get(i).toString())) << ", "
             << getPrintablePerm((pm_queries->get(i))) << "." << endl;
    }

    delete poss;
}
