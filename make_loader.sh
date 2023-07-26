#!/bin/sh
cxx="g++"
#"$cxx" loader/main.cpp -o loader -ldl \
	#-std=c++20 \
	#-Wall -Wextra -Wshadow -Wswitch-enum \
	#moth.lib \
	#|| exit 1
lib_path='/home/temmel/.local/share/moth'

"$cxx" \
	-o $lib_path/loader \
	-ldl \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	loader/main.cpp \
	-L$lib_path \
	-lmoth \
	-Wl,-rpath -Wl,$lib_path \
	|| exit 1
