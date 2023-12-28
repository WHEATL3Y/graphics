#!/bin/bash
gcc -o main -I /usr/include/cairo/ main.c -l cairo -lX11
