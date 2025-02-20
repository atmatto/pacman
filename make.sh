#!/bin/bash

mkdir -p out
qmake -o out/Makefile && {
	cd out
	bear -- make
}
