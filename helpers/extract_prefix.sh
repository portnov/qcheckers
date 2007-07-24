#!/bin/sh
grep "define PREFIX" common.h | cut -d'"' -f2
