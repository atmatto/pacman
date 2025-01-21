#!/bin/bash

qmake -o out/Makefile && {
	cd out
	bear -- make
}
