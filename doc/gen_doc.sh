#!/usr/bin/sh

./collect_sources.sh
cd Src
doxygen ../doxyfile.txt
cd ..
