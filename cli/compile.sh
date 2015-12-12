#!/bin/sh

OPENSSL_DIR="/usr/local/ssl"

OPENSSL_LIB="${OPENSSL_DIR}/lib"
OPENSSL_INCLUDE="${OPENSSL_DIR}/include/openssl"

g++ -c src/contracts.cpp -o src/contracts.o
g++ -c src/passwords.cpp -o src/passwords.o

g++ src/*.o $OPENSSL_LIB/*.a -I$OPENSSL_INCLUDE  main.cpp -o ./../cli_executable

