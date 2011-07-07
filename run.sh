#!/bin/sh
g++ -o cba2csv cba2csv.cpp -O3 -l podofo -l freetype -l fontconfig -l pthread -l jpeg && ./cba2csv test.pdf
