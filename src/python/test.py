#!/usr/bin/env python
import sys
import unittest


class MinimumCoins(unittest.TestCase):
    """
    Find minimum number of coins that make a given value
    Given a value V, if we want to make change for V cents, and we have infinite supply of each of C = { C1, C2, .. , Cm} valued coins
    what is the minimum number of coins to make the change?
    """

    @staticmethod
    def update_result(result, coin):
        result[coin] = result[coin] + 1 if coin in result else 1

    def simple_solution(self, coins, value):
        def solve(coins, num_of_coins, value):
            # print(value)
            if value == 0:
                return 0
            result = sys.maxsize
            for i in range(num_of_coins):
                if coins[i] <= value:
                    # the coin value < value
                    # if use this coin, the target value is the original value minus this coin's value
                    res = solve(coins, num_of_coins, value - coins[i])

                    if (
                        res != sys.maxsize and res + 1 < result
                    ):  # do we have a solution? and if we have a solution does adding one coin (this coin) still less than current result
                        result = res + 1
            return result

        return solve(coins, len(coins), value)

    def dp_solution(self, coins, value):
        num_of_coins = len(coins)
        cache = [0] + [sys.maxsize for i in range(value)]

        for v in range(1, value+1):
            for i in range(num_of_coins):
                if coins[i] <= v:
                    res = cache[v-coins[i]]
                    if (
                            res != sys.maxsize and res + 1 < cache[v]
                    ):  # do we have a solution? and if we have a solution does adding one coin (this coin) still less than current result
                        cache[v] = res + 1

        return cache[value]

    @staticmethod
    def gen_test_cases():
        return (
            # (sorted list coins that can be used, value V, minimum coins)
            ([1, 5, 6, 9], 0, 0),
            ([9, 6, 5, 1], 11, 2),
            ([1, 3, 6], 11, 4),
        )

    def test_simple_solution(self):
        for coins, value, result in self.gen_test_cases():
            self.assertEqual(self.simple_solution(coins, value), result)

    def test_dp_solution(self):
        for coins, value, result in self.gen_test_cases():
            self.assertEqual(self.dp_solution(coins, value), result)


if __name__ == "__main__":
    unittest.main()
