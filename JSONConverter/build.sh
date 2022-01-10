#!/usr/bin/sh
mkdir Build
cd Build
cmake .. -DJSON_TESTS=ON
cmake --build .
cd ..
