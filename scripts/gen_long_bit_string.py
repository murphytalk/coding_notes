#!/usr/bin/python
from __future__ import print_function
import random


def gen(num):
    s = ''
    for i in range(num):
        s += random.choice(('0', '1'))
    print(s)


if __name__ == '__main__':
    gen(10240)
    gen(10250)
