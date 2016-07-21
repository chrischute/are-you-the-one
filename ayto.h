/**
 * ayto.h
 * Christopher Chute
 *
 * Header for Are You The One? simulator.
 */

struct args {
    std::vector<std::string> *perms;
    std::vector<std::string> *chunk;
    std::pair<std::string, int> *cd;
    int thread_id;
};

int evaluate(const std::string &sol, const std::string &query);
std::vector<std::string> remove_perms(std::vector<std::string> &perms, int eval, std::string &query);
std::pair<std::string, int> guess_tb(std::vector<std::string> &perms, std::vector<std::string> &guessed_tb, int turn);
std::pair<std::string, int> guess_pm(std::vector<std::string> &perms, std::vector<std::string> &guessed, int turn);
void make_chunks(std::vector<std::string> &orig, std::vector<std::vector<std::string> > &chunks, int n);
std::string min_candidate(std::pair<std::string, int> *candidates, int n);
void get_score(struct args *args);
int wc_response(std::string &guess, std::vector<std::string> &perms);
bool prcmp(std::pair<int, int> x, std::pair<int, int> y);
std::string get_worst_sequence(int sz);
void sequence_print(std::string s);
struct args **create_args(std::vector<std::string> &perms, std::pair<std::string, int> *cd, std::vector<std::string> &chunk, int thread_id);
