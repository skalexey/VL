#!/bin/bash

deps_scenario()
{
	source dependencies.sh
	source deps_config.sh

	download_dependency "Utils" "$depsLocation" "git@github.com:skalexey/Utils.git"
	source "$depsLocation/VL/JSONConverter/deps_scenario.sh"
}

deps_scenario $@