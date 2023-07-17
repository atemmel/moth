#!/bin/sh
cxx="x86_64-w64-mingw32-c++"
#cxx="g++"
#"$cxx" loader/main.cpp -o loader -ldl \
	#-std=c++20 \
	#-Wall -Wextra -Wshadow -Wswitch-enum \
	#moth.lib \
	#|| exit 1

mkdir -p build
cp vendor/lib/SDL2.dll build/SDL2.dll

"$cxx" \
	-static \
	-static-libgcc \
	-static-libstdc++ \
	-Wl,-Bstatic -lwinpthread \
	loader/main.cpp \
	-o build/loader \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	-D_WIN32 -D_MING \
	moth.dll \
	|| exit 1
