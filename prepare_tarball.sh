#!/bin/bash

VERSION=0.9.0

#make clean

tar cJ --exclude-vcs --exclude=./debian --exclude=./src/qcheckers -f ../qcheckers_$VERSION.orig.tar.xz . 
