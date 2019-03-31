#!/usr/bin/python
from __future__ import print_function
import random
# from sys import maxsize


def gen(num, N):
    for i in range(num):
        print(random.randint(1, N))


if __name__ == '__main__':
    count = 1024*1024
    gen(count, count*10)
