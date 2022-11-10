#!/bin/sh
g++ -shared -fPIC *.cpp -o libmoth.so \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	-lSDL2
