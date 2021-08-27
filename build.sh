#!/bin/bash
for f in *.cpp
do
	g++ -std=c++20 -O2 $f -o bin/${f: 0:-4}
done
