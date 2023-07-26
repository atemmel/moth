#!/bin/sh
echo "make_dyn.sh"
cxx="g++"
lib_path='/home/temmel/.local/share/moth'
# Unix flags:
#-Wl,-export-dynamic \
"$cxx" \
	-fPIC \
	-shared \
	-o build/MyActor.dll \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	-Idynamic \
	-Ilib \
	dynamic/MyActor.cpp \
	-L$lib_path \
	-lmoth \
