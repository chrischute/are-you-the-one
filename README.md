# How to beat *Are You The One?*
*Are You The One?* is a reality TV series on MTV. As stated on the [*Are You The One?* web page](http://www.mtv.com/shows/are-you-the-one), this show is "the most ambitious dating experiment ever attempted." The contestants are "10 gorgeous single women and 10 hot single guys," who are placed together in Hawaii. MTV selects the contestants in soulmate pairings, but does not reveal these pairings until the end of the show. Instead, the contestants must determine the 10 perfect matches themselves. As if letting love slip through one's fingers weren't scary enough, failing to find the perfect matching causes the entire group to forgo one million US dollars.

Our goal is to prove that true love can always be found on *Are You The One?*, *i.e.*, nobody should ever lose. The contestants just need to spin up ```gcc``` and run this multi-threaded C++ simulation of a simple guessing algorithm. No fortune telling or couple's therapy sessions needed.

## Rules of *Are You The One?*
At the end of each week, the contestants go through two rounds of guessing:
  * **Truth Booth:** A single pair of contestants is submitted to the "truth booth."
     * *Feedback:* "Yes" or "no," indicating whether these two contestants are a match. 
  * **Perfect Matching:** The entire group agrees on a perfect matching, *i.e.,* a list of the 10 pairs they believe to be soulmates.
    * *Feedback:* A number 0 through 10, indicating the number of pairs that are correct.

They use this feedback, along with other events on the show, to inform the following week's guess. If they can determine the hidden matching in 10 weeks or fewer, they split $1 million.
## Minimax Guessing Algorithm
```ayto.cpp``` uses four stages of guessing, all based on the [minimax algorithm](http://www.cs.uni.edu/~wallingf/teaching/cs3530/resources/knuth-mastermind.pdf):
  1. **Fixed Guess:** The first few guesses are fixed.
  2. **Informed Guess:** After the first few guesses, the simulator chooses a guess from a pool of about 10,000 fixed possibilities. It considers the worst-case response, *i.e.,* the response eliminating the fewest possible answers. The number of answers remaining given this worst-case response is the guess's *minimax score*. The simulator then submits the guess with the lowest minimax score.
  3. **Partial Minimax:** When still fewer possible answers remain, the possible answers themselves are assigned their minimax score. The possible answer with the lowest minimax score is submitted as the next guess.
  4. **Full Minimax:** In this end-game stage, all 10! possible guesses are assigned their minimax score, and again the guess with the lowest score is submitted.

Methods (2), (3), and (4) assign scores in parallel using a pool of about 10 threads.