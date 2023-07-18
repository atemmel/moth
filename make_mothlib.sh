#!/bin/sh
cxx="x86_64-w64-mingw32-c++"
#cxx="g++"
"$cxx" \
	-fPIC \
	-shared \
	-o build/libmoth.dll \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	-D_WIN32 -D_MING \
	-Ivendor/include \
	moth/*.cpp \
	-Lvendor/lib \
	-lSDL2 \
	-Wl,--out-implib,build/libmoth.dll.lib \
