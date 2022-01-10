@ECHO off

set build=Build\
set deps=dependencies\

IF not exist %deps% ( mkdir %deps% )

IF not exist %deps%\rapidjson (
	cd %deps%
	git clone https://github.com/Tencent/rapidjson.git
	cd ..
)

IF not exist %build% ( mkdir %build% )
cd Build
cmake .. -DJSON_TESTS=ON
cmake --build .
cd ..
