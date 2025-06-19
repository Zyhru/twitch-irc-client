#!/bin/sh

echo "Running"
gcc curl.c -o main -lcurl
./main
