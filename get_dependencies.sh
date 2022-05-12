#!/bin/bash

enterDirectory=${PWD}

if [ ! -z "$1" ]; then 
	echo "Go to the source directory passed: '$1'"
	cd "$1" # go to the source directory passed
fi

folderName=${PWD##*/}

source log.sh

log_prefix="-- [${folderName} get_dependencies script]: "

log "Check for dependencies" " -"

if [ ! -f "deps_config.sh" ]; then
	log "No dependencies" " -"
	exit
fi
source deps_config.sh

source deps_scenario.sh $@
retval=$?
if [ $retval -ne 0 ]; then
	log "Error occured during the deps_scenario.sh execution " " -"
	cd "${enterDirectory}"
	exit 1
fi

cd "${enterDirectory}"