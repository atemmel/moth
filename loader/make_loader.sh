#!/bin/sh
g++ main.cpp -o loader -ldl \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	../moth/libmoth.so \
	|| exit 1
