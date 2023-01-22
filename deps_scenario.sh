#!/bin/bash

deps_scenario()
{
	local THIS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
	source $THIS_DIR/dependencies.sh
	source $THIS_DIR/deps_config.sh

	download_dependency "Utils" "$depsLocation" "git@github.com:skalexey/Utils.git"
	source $THIS_DIR/"$depsLocation/VL/JSONConverter/deps_scenario.sh"
}

deps_scenario $@