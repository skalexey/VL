mklink /D VL ..
mkdir Build
cd Build
cmake .. -DJSON_TESTS=ON
cmake --build .
cd ..
