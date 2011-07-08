#!/bin/sh
# My home machine
#g++ -o cba2csv cba2csv.cpp -O3 -march=barcelona -DNDEBUG -l podofo -l freetype -l fontconfig -l pthread -l jpeg && ./cba2csv test.pdf

# General (all cpus) and optimized
g++ -o cba2csv cba2csv.cpp -O3 -DNDEBUG -l podofo -l freetype -l fontconfig -l pthread -l jpeg && ./cba2csv test.pdf

# Debug
#g++ -o cba2csv cba2csv.cpp -O0 -g -DDEBUG -l podofo -l freetype -l fontconfig -l pthread -l jpeg && ./cba2csv test.pdf
