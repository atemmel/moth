#!/bin/sh

mkdir -p build

./make_mothlib.sh \
	&& ./make_loader.sh \
	&& ./make_dyn.sh \
