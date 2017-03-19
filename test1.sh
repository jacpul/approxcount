#!/bin/bash

rm data*.txt
THREADS="1 2 4 8 16"

echo "Sloppy counting runtime as threads increase ($THREADS)"
for threads in $THREADS
do 
	./sloppycount -n 1000000000 -s 1000 -r 10 -m sloppy -t $threads > data${threads}.txt
done

paste data1.txt data2.txt data4.txt data8.txt data16.txt
