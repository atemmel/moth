#!/bin/sh
echo "make_dyn.sh"
cxx="g++"
moth_path='/home/temmel/.local/share/moth'
# Unix flags:
#-Wl,-export-dynamic \
"$cxx" \
	-fPIC \
	-shared \
	-o build/shared.dll \
	-std=c++20 \
	-Wall -Wextra -Wshadow -Wswitch-enum \
	-Idynamic \
	-I$moth_path/include \
	dynamic/*.cpp \
	-L$moth_path/bin \
	-lmoth \
	|| exit 1
