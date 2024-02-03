#!/bin/bash
mkdir -p build
cd build
cmake -S ../ -B .
make && make Shaders && 	__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./LveEngine
# If you aren't on linux or want to run on integratet or non nvdia gpu, use the following command.
# make && make Shaders && ./LveEngine

