#!/bin/sh
echo "Building $1"
g++ -fPIC MyActor.cpp -shared -o $1 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	-Wl,-export-dynamic
