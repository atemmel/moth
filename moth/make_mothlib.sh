#!/bin/sh
g++ -shared -fPIC *.cpp -o libmoth.so \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	-lSDL2
