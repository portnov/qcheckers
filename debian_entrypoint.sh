#!/bin/bash

set -e

./prepare_tarball.sh

debuild -uc -us

mv ../qcheckers*.deb /dst/
