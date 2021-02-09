#!/usr/bin/env bash

set -e # fail out if any step fails

. setCompilePath.sh

compile()
{
	module=${1}
	echo Compiling ${module}
	cd ${MODULESROOTPATH}/${module}
	source compile.sh 
}

mkdir -p ${INSTALLDIR}
mkdir -p ${INSTALLDIR}/lib
mkdir -p ${INSTALLDIR}/bin
mkdir -p ${INSTALLDIR}/include
export MODULESROOTPATH=$(pwd)
for module in Modules/* 
do
        compile ${module}
done
