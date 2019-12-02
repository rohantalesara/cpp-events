#!/bin/bash
g++ -c -std=c++11 *.cpp
g++  -std=c++11 *.o -pthread
valgrind --tool=memcheck ./a.out 
