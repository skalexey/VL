#!/usr/bin/sh
mkdir Build
cd Build
cmake .. -DVL_TESTS=ON
cmake --build .
cd ..
