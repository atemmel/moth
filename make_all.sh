#!/bin/sh

mkdir -p build

echo \
	&& echo mothlib && ./make_mothlib.sh \
	&& echo loader && ./make_loader.sh \
	&& echo dynamic && ./make_dyn.sh \
