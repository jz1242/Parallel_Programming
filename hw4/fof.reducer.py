#!/usr/bin/env python
'''
Jason Zhang
jzhan127

'''
import sys


def main():
    count = 0
    temp = None
    for inp in sys.stdin:
        if inp == temp:
            count = count + 1
        else:
            count = 0
        if count == 1:
            s = inp.split('\t')
            print(s[0])
        temp = inp


if __name__ == '__main__':
   main()