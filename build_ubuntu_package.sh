#!/bin/bash
set -e

cp debian/control.ubuntu debian/control
rm Makefile src/Makefile
qmake
make clean
rm Makefile src/Makefile

docker build -t qcheckers-ubuntu -f Dockerfile.ubuntu .

docker run --rm --name qcheckers-ubuntu -v $(pwd):/src -v $(pwd):/dst qcheckers-ubuntu
