#!/usr/bin/env bash

javac CoinFlip.java

flip=1000000000
threads=32
for (( i = 1; i <= $threads; i*=2 )); 
do
	java CoinFlip $threads $flip >>data.txt
done
