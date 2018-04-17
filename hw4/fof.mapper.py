#!/usr/bin/env python
'''
Jason Zhang
jzhan127
'''
import sys


def main():
    for inp in sys.stdin:
        people = inp.split()
        for i in range(1, len(people)):
            for j in range(1, len(people)):
                if(i != j):
                    if(int(people[0]) < int(people[j])):
                        print(people[i]+" "+people[0]+" "+people[j]+"\t"+"1")
                    else:
                        print(people[i]+" "+people[j]+" "+people[0]+"\t"+"1")

if __name__ == '__main__':
   main()