# How to beat *Are You The One?*
*Are You The One?* is a reality TV series on MTV. As stated on the [*Are You The One?* web page](http://www.mtv.com/shows/are-you-the-one), this show involves "10 gorgeous single women and 10 hot single guys," who are placed together in Hawaii to find their true love. It really is "the most ambitious dating experiment ever attempted." MTV selects the contestants in matched pairs, but does not reveal these matches until the end of the show. Instead, the contestants must correctly match themselves using only feedback from guesses made at the end of each week. But the excitement doesn't stop there &mdash; as if letting love slip through one's fingers weren't punishment enough, failing to find the perfect matching causes the group to forgo one million US dollars.

Our goal is to prove that true love can always be found on *Are You The One?*, *i.e.*, nobody should ever lose. The contestants just need to spin up ```g++``` and run this multi-threaded, excessively optimized guess generator. It uses a relatively simple guessing algorithm &mdash; No fortune telling or couple's therapy sessions needed.

## Try It Out
Clone this repo, write down a permutation of *{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}*, then run ```./ayto -i``` in the top-level directory. Now just answer the questions as prompted. (This permutation corresponds to exactly one matching, gotten by labeling each gender 0 through 9, writing the women in order, then matching the men to the women. The order of the men describes the matching.)

## Rules of *Are You The One?*
At the end of each week, the contestants go through two rounds of guessing:
  * **Truth Booth:** The group chooses a single pair of contestants to enter the "truth booth."
     * *Feedback:* "Yes" or "no," indicating whether these two contestants are a match. 
  * **Perfect Matching:** The entire group agrees on a perfect matching, *i.e.*, a list of the 10 pairs they believe to be soulmates.
    * *Feedback:* A number 0 through 10, indicating the number of pairs that are correct.

They can use this feedback, along with more personal happenings throughout the week, to inform the following week's guess. If they can determine the hidden matching in 10 weeks or fewer, they split $1 million.
## Minimax Guessing Algorithm
```ayto.cpp``` picks its guesses using a variant of the [minimax algorithm](http://www.cs.uni.edu/~wallingf/teaching/cs3530/resources/knuth-mastermind.pdf). In this strategy, a potential guess *g* is assigned a *minimax score* as follows: Take the set *A* of answers that have not yet been ruled out by previous feedback. For each possible response *r &isin; {0, 1, ..., 10}* (*r =* number correct pairs in the guess *g*), compute the number *n<sub>r</sub>* of solutions that would remain given feedback *r*. So *n<sub>r</sub>* is computed by filtering *A* such that only answers with *r* pairs in common with *g* remain, then counting this filtered set. Let *N* be the maximum such *n<sub>r</sub>* for the guess *g*. This *N* is called the *minimax score* for *g*.

```ayto.cpp``` uses the minimax score in four stages of guessing:
  1. **Fixed Guess:** The first few guesses are predetermined strings.
  2. **Informed Guess:** After the first few guesses, the simulator considers a pool of about 10,000 fixed possibilities and chooses the guess with the lowest minimax score.
  3. **Partial Minimax:** When still fewer possible answers remain, the possible answers themselves are assigned their minimax score. The possible answer with the lowest minimax score is submitted as the next guess.
  4. **Full Minimax:** In this end-game stage, all 10! possible guesses are assigned their minimax score, and again the guess with the lowest score is submitted.

When using methods (2), (3), and (4), ```ayto.cpp``` computes scores in parallel across many threads.