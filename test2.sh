#!/bin/bash

rm data*.txt

echo "Sloppy counting runtime as slop value increases"
for slop in 100 200 300 400 500 600
do
	./sloppycount -n 1000000000 -s $slop -r 10 -m sloppy -t 4 > data${slop}.txt
done

paste data100.txt data200.txt data300.txt data400.txt data500.txt data600.txt
