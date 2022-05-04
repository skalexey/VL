#!/bin/sh

source dependencies.sh
source deps_config.sh

download_dependency "VL" "$depsLocation" "git@github.com:skalexey/VL.git"
cd "VL"

download_dependency "rapidjson" "$depsLocation" "https://github.com/Tencent/rapidjson.git"

