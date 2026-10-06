/* Q10: Greedy Superstring experiment (shortest common superstring is NP-hard)
 * GREEDY: drop strings contained in others; repeatedly merge the ordered pair with maximum overlap
 *         (suffix of a == prefix of b); with no overlap left, concatenate.
 * EXACT (n<=16): Held-Karp bitmask DP, dp[mask][last] = shortest superstring covering mask and ending in last.
 * Prints |greedy|, |optimal| and the ratio (conjecture: ratio <= 2; the sheet's claimed Sept-2026 disproof
 * is unverified and its instances are too large for the exact solver).
 * Complexity: greedy O(n^3 * total length); exact DP O(2^n n^2) time, O(2^n n) space.
 * Input: n, then n strings (<=60 chars)
 */
