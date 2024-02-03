#!/bin/bash
mkdir -p build
cd build
cmake -S ../ -B .
make && make Shaders && 	__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./LveEngine
```
