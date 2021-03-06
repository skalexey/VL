@ECHO off

set build=Build-cmake
set buildConfig=Debug
set buildFolderPrefix=Build
set deps=dependencies\
set cmakeTestsArg= -DJSON_TESTS=ON
set cmakeGppArg=

for %%x in (%*) do (
	set /A argCount+=1
	if "%%~x"=="only-deps" (
		echo --- 'only-deps' option passed. Download dependencies and skip the build process
		set onlyDownload=d
		goto download
	) else if "%%~x"=="only-lib" (
		echo --- 'only-lib' option passed. Build only library without tests
		set cmakeTestsArg=
     ) else if "%%~x" == "g++" (
	     echo --- 'g++' option passed. Build with g++ compiler
	     set cmakeGppArg = -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gpp
	     set buildFolderPrefix=Build-g++
     ) else if "%%~x" == "release" (
		echo --- 'release' option passed. Set Release build type
		set buildConfig=Release
	) 
)

: download

IF not exist %deps% ( mkdir %deps% )

IF not exist %deps%\rapidjson (
	echo --- Download rapidjson from GitHub
	cd %deps%
	git clone https://github.com/Tencent/rapidjson.git
	cd ..
) else (
	echo --- rapidjson is already downloaded
)

if defined onlyDownload ( goto end )

if not exist %build% (
	mkdir %build%
	if %errorlevel% == 0 ( echo --- '%build%' directory created
	) else ( echo --- Error while creating '%build%' directory. Exit 
		goto end )
) else ( echo --- '%build%' directory already exists )

cd %build%

echo --- Configure JSONConverter with CMake

cmake ..%cmakeGppArg%%cmakeTestsArg%

if %errorlevel% neq 0 (
	echo --- CMake generation error: %errorlevel%
	goto end
)

echo --- Build JSONConverter with CMake

cmake --build . --config=%buildConfig%

if %errorlevel% neq 0 (
	echo --- CMake build error: %errorlevel%
	goto end
) else (
	echo --- CMake build successfully completed
)
cd ..



: end
