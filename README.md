# How to beat *Are You The One?*
*Are You The One?* is a reality TV series on MTV. As stated on the [*Are You The One?* web page](http://www.mtv.com/shows/are-you-the-one), this show involves "10 gorgeous single women and 10 hot single guys," who are placed together in Hawaii to find their true love. MTV selects the contestants in matched pairs, but does not reveal these matches until the end of the show. Instead, the contestants must guess the perfect matching using information from the Truth Booth and Perfect Matching ceremonies (see below). If they can guess the 10 pairs by the end of the 10<sup>th</sup> week, they get $1 million to split between them.

The goal of this project is to show that true love can always be found on *Are You The One?*, *i.e.*, nobody should ever lose. The contestants just need to spin up ```g++``` and run this guess generator. It uses a relatively simple guessing algorithm &mdash; No fortune telling or couple's therapy sessions needed.

## Try It Out
Clone this repo and  run ```make``` followed by ```./ayto``` in the top-level directory. This will generate a random matching of the last season's contestants, then submit guesses until the program wins. Please contact the owner of this page if you find a matching that takes more than 10 weeks to solve.

You can also use the following flags:
  * ```-a``` All Matchings Mode: Run on all possible matchings (could take many years).
  * ```-f <filename>``` File Mode: Run on answers read from a file. The file's format should be plain text, with a single permutation of ```0123456789``` on each line.
  * ```-i``` Interactive Mode: You choose the answer, and you give the feedback.
  * ```-n``` Numbers Mode: Print numbers instead of names.
  * ```-v``` Verbose Mode: Print more to the console.

## Weekly Guessing Ceremony
At the end of each week, the contestants go through two rounds of guessing:
  * **Truth Booth:** The group chooses a single pair of contestants to enter the "truth booth."
     * *Feedback:* "Yes" or "no," indicating whether these two contestants are a match. 
  * **Perfect Matching:** The entire group agrees on a perfect matching, *i.e.*, a list of the 10 pairs they believe to be soulmates.
    * *Feedback:* A number 0 through 10, indicating the number of pairs that are correct.

## Minimax Guessing Algorithm
```ayto.cpp``` picks its guesses using a variant of the [minimax algorithm](http://www.cs.uni.edu/~wallingf/teaching/cs3530/resources/knuth-mastermind.pdf). In this strategy, a potential guess *g* is assigned a *minimax score* as follows: Take the set *A* of answers that have not yet been ruled out by previous feedback. For each possible response *r &isin; {0, 1, ..., 10}* (*r =* number correct pairs in the guess *g*), compute the number *n<sub>r</sub>* of solutions that would remain given feedback *r*. So *n<sub>r</sub>* is computed by filtering *A* such that only answers with *r* pairs in common with *g* remain, then counting this filtered set. Let *N* be the maximum such *n<sub>r</sub>* for the guess *g*. This *N* is called the *minimax score* for *g*.

```ayto.cpp``` uses the minimax score in four stages of guessing:
  1. **Fixed Guess:** The first few guesses are predetermined strings.
  2. **Informed Guess:** After the first few guesses, the simulator considers a pool of about 10,000 fixed possibilities and chooses the guess with the lowest minimax score.
  3. **Partial Minimax:** When still fewer possible answers remain, the possible answers themselves are assigned their minimax score. The possible answer with the lowest minimax score is submitted as the next guess.
  4. **Full Minimax:** In this end-game stage, all 10! possible guesses are assigned their minimax score, and again the guess with the lowest score is submitted.

When using methods (2), (3), and (4), ```ayto.cpp``` computes scores in parallel across many threads.