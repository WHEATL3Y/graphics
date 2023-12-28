#!/bin/bash
gcc -o main $(pkg-config --cflags --libs cairo) main.c
