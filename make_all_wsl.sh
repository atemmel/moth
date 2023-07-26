#!/bin/sh

mkdir -p build

./make_mothlib_wsl.sh \
	&& ./make_loader_wsl.sh \
	&& ./make_dyn_wsl.sh \
