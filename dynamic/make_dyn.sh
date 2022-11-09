#!/bin/sh
#g++ -fPIC my_actor.cpp -shared -o my_actor.so \
echo "Building $1"
g++ -fPIC my_actor.cpp -shared -o $1 \
	-Wl,--whole-archive -Wl,--no-whole-archive -Wl,-export-dynamic
