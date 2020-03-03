#!/usr/bin/env bash

# Created by Sian Croser
#
# Used to remove relevant compiled code so that a new build will compile src changes
#
# While 'make clean' will have the same effect, it will also clear the gsoap
# compiled code which can be time consuming to recompile.

rm ./src/services/*.o
rm ./src/*.o
