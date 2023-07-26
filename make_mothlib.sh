#!/bin/sh
cxx="g++"

lib_path='/home/temmel/.local/share/moth'

"$cxx" \
	-fPIC \
	-shared \
	-o $lib_path/libmoth.a \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	lib/*.cpp \
	-ldl \
	-lSDL2 \
	-Wl,--out-implib,$lib_path/libmoth.a.so \
