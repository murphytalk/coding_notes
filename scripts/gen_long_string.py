#!/usr/bin/python
from __future__ import print_function
import random


def gen(num):
    s = ''
    for i in range(num):
        s += chr(random.randint(ord('a'), ord('z')))
    print(s)


if __name__ == '__main__':
    gen(1024*1024)
