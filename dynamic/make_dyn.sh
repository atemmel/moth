#!/bin/sh
echo "Building $1"
g++ -fPIC my_actor.cpp -shared -o $1 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	-Wl,-export-dynamic
