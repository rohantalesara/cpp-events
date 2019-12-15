#!/bin/bash
g++ -c -std=c++11 *.cpp
g++  -std=c++11 *.o -pthread
./a.out 
rm *.o a.out
