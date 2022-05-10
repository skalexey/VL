#!/bin/sh

echo " --- Check for dependencies"

if [ ! -f "deps_config.sh" ]; then
	echo " ---- No dependencies"
	exit
fi
source deps_config.sh

enterDirectory=${pwd}

source deps_scenario.sh
retval=$?
if [ $retval -ne 0 ]; then
	echo " --- Error occured during the deps_scenario.sh execution"
	cd "${enterDirectory}"
	exit 1
fi

cd "${enterDirectory}"