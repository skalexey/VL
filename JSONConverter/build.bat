@ECHO off

set build=Build\
set deps=dependencies\
set jsonTestsArg= -DJSON_TESTS=ON
set cmakeGppArg=

for %%x in (%*) do (
	set /A argCount+=1

	if "%%~x"=="only-lib" (
		echo "'only-lib' option passed. Build only library without tests" 
		set jsonTestsArg=
     ) else if "%%~x" == "g++" (
	     echo "'g++' option passed. Build with g++ compiler"
	     set cmakeGppArg = -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gpp
	     set build=Build-g++\
     )
)
IF not exist %deps% ( mkdir %deps% )

IF not exist %deps%\rapidjson (
	cd %deps%
	git clone https://github.com/Tencent/rapidjson.git
	cd ..
)

IF not exist %build% ( mkdir %build% )
cd Build
cmake ..%cmakeGppArg%%jsonTestsArg%
cmake --build .
cd ..
