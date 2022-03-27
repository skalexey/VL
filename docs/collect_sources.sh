#!/usr/bin/sh

rm -rf Src

mkdir Src

includePath="../include/"
srcPath="../src/"
jsonConverterPath="../JSONConverter"
jsonInclude="$jsonConverterPath/include"
jsonSrc="$jsonConverterPath/src"

cd Src
	cp -r "../$includePath" VL
	cd VL
		cp -r "../../$srcPath" src
	cd ..
	mkdir Extensions
	cd Extensions
		cp -r "../../$jsonInclude" JSONConverter
		cp -r "../../$jsonSrc" JSONConverter/src
	cd ..
	
	mkdir Utility
	mv VL/Utils.h Utility
	mv VL/Log.h Utility
	
	mkdir Utility/src
	mv VL/src/Utils.cpp Utility/src
cd ..
