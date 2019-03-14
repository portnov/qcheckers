#!/bin/bash
set -e

docker build -t qcheckers-builder -f Dockerfile.debian .

docker run --rm --name qcheckers-builder -v $(pwd):/src -v $(pwd):/dst qcheckers-builder
