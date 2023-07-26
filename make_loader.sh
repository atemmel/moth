#!/bin/sh
cxx="g++"
#"$cxx" loader/main.cpp -o loader -ldl \
	#-std=c++20 \
	#-Wall -Wextra -Wshadow -Wswitch-enum \
	#moth.lib \
	#|| exit 1
moth_path='/home/temmel/.local/share/moth'

"$cxx" \
	-g \
	-o $moth_path/bin/loader \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	loader/main.cpp \
	-I$moth_path/include \
	-L$moth_path/bin \
	-ldl \
	-lmoth \
	-Wl,-rpath -Wl,$moth_path/bin \
	|| exit 1
