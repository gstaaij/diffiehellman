#!/bin/sh

set -xe

gcc -Wall -Wextra -o build/diffiehellman diffiehellman.c
x86_64-w64-mingw32-gcc -Wall -Wextra -o build/diffiehellman diffiehellman.c
