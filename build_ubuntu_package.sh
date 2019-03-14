#!/bin/bash
set -e

cp debian/control.ubuntu debian/control
rm -f Makefile src/Makefile
qmake
make clean
rm -f Makefile src/Makefile

docker build -t qcheckers-ubuntu -f Dockerfile.ubuntu .

docker run --rm --name qcheckers-ubuntu -v $(pwd):/src -v $(pwd):/dst qcheckers-ubuntu
