// Functions for interacting with the contracts
#ifndef _CONTRACTS_H
#define _CONTRACTS_H
#include <string>
using namespace std;

string get_first_four(string method);
string to_bytes(char* x);
string get_contract_address();

string build_contract(char* seed);



#endif
