#!/bin/bash
set -e

cp debian/control.debian debian/control
rm Makefile src/Makefile
qmake
make clean
rm Makefile src/Makefile

docker build -t qcheckers-debian -f Dockerfile.debian .

docker run --rm --name qcheckers-debian -v $(pwd):/src -v $(pwd):/dst qcheckers-debian
