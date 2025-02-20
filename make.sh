#!/bin/bash

mkdir out
qmake -o out/Makefile && {
	cd out
	bear -- make
}
