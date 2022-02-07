@echo off

set buildType=Debug
set buildFolderPrefix=Build
set cmakeTestsArg= -DVL_TESTS=ON
set cmakeGppArg=

for %%x in (%*) do (
	set /A argCount+=1
	if "%%~x"=="only-lib" (
		echo --- 'only-lib' option passed. Build only library without tests
		set cmakeTestsArg=
     ) else if "%%~x" == "g++" (
	     echo --- 'g++' option passed. Build with g++ compiler
	     set cmakeGppArg= -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gpp
	     set buildFolderPrefix=Build-g++
     )
)

set build=%buildFolderPrefix%-cmake-%buildType%\

if not exist %build% (
	mkdir %build%
	if %errorlevel% == 0 ( echo --- '%build%' directory created
	) else ( echo --- Error while creating '%build%' directory. Exit 
		goto end )
) else ( echo --- '%build%' directory already exists )

cd %build%

echo --- Configure VL with CMake

cmake .. "-DCMAKE_BUILD_TYPE:STRING=%buildType%"%cmakeGppArg%%cmakeTestsArg%

if %errorlevel% neq 0 (
	echo --- CMake generation error: %errorlevel%
	goto end
)

echo --- Build VL CMake

cmake --build .

if %errorlevel% neq 0 (
	echo --- CMake build error: %errorlevel%
	goto end
) else (
	echo --- CMake build successfully completed
)
cd ..


