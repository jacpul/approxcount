#!/bin/bash

rm data*.txt

echo "Runtime as threads increase (mode: random1)"
for threads in 1 2 4 8 16
do 
	./sloppycount -n 1000000000 -s 1000 -r 10 -m random1 -t $threads > data${threads}.txt
done
paste data1.txt data2.txt data4.txt data8.txt data16.txt
