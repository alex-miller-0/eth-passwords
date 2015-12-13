#!/bin/sh

OPENSSL_DIR="/usr/lib"

g++ -c src/contracts.cpp -o src/contracts.o
g++ -c src/passwords.cpp -o src/passwords.o

g++ src/*.o -L$OPENSSL_DIR -lssl -lcrypto   main.cpp -o cli_executable
