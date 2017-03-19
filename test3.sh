#!/bin/bash

rm data*.txt

echo "Runtime as threads increase for non-sloppy counting"
for threads in 1 2 4 8
do 
	./sloppycount -n 10000000 -s 1 -r 10 -m noslop -t $threads > data${threads}.txt
done

paste data1.txt data2.txt data4.txt data8.txt
