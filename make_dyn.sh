#!/bin/sh
echo "make_dyn.sh"
cxx="x86_64-w64-mingw32-c++"
#cxx="g++"
# Unix flags:
#-Wl,-export-dynamic \
"$cxx" \
	-fPIC \
	-shared \
	-o build/MyActor.dll \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	-D_WIN32 -D_MING \
	-Idynamic \
	-Ilib \
	dynamic/MyActor.cpp \
	-Lbuild \
	-lmoth \
