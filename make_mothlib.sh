#!/bin/sh
cxx="g++"

moth_path='/home/temmel/.local/share/moth'

mkdir -p $moth_path/include
mkdir -p $moth_path/bin

cp lib/*.hpp $moth_path/include/

"$cxx" \
	-g \
	-fPIC \
	-shared \
	-o $moth_path/bin/libmoth.a \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	lib/*.cpp \
	-ldl \
	-lSDL2 \
	-Wl,--out-implib,$moth_path/bin/libmoth.a.so \
