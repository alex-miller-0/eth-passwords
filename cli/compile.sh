g++ -c src/contracts.cpp -o src/contracts.o
g++ -c src/passwords.cpp -o src/passwords.o

g++ src/*.o main.cpp -o main

