#!/bin/sh
cxx="x86_64-w64-mingw32-c++"
#cxx="g++"
"$cxx" \
	-fPIC \
	-shared \
	-static-libgcc \
	-static-libstdc++ \
	-Wl,-Bstatic -lwinpthread \
	-Wl,-Bdynamic \
	-o build/moth.dll \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	-D_WIN32 -D_MING \
	-Ivendor/include \
	moth/*.cpp \
	-Lvendor/lib \
	-lSDL2
