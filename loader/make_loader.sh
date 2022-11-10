#!/bin/sh
g++ main.cpp -o loader -ldl \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	../moth/libmoth.so \
	|| exit 1
