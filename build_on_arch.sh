#!/bin/bash
set -e

rm -f Makefile src/Makefile
qmake
make clean
rm -f Makefile src/Makefile

docker build -t qcheckers-arch -f Dockerfile.arch .

docker run --rm --name qcheckers-arch -v $(pwd):/src/qcheckers -v $(pwd):/dst qcheckers-arch
