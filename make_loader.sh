#!/bin/sh
cxx="x86_64-w64-mingw32-c++"
#cxx="g++"
#"$cxx" loader/main.cpp -o loader -ldl \
	#-std=c++20 \
	#-Wall -Wextra -Wshadow -Wswitch-enum \
	#moth.lib \
	#|| exit 1

STDCPP=/usr/x86_64-w64-mingw32/bin/libstdc++-6.dll
STDC=/usr/x86_64-w64-mingw32/bin/libgcc_s_seh-1.dll
PTHREADS=/usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll

mkdir -p build
cp vendor/lib/SDL2.dll build/SDL2.dll
cp $STDCPP build/libstdc++-6.dll
cp $STDC build/libgcc_s_seh-1.dll
cp $PTHREADS build/libwinpthread-1.dll

"$cxx" \
	-o build/loader \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	-D_WIN32 -D_MING \
	loader/main.cpp \
	-Lbuild \
	-llibmoth.dll \
	|| exit 1
