// Functions for interacting with the contracts
#ifndef _CONTRACTS_H
#define _CONTRACTS_H
#include <string>
using namespace std;

string get_first_four(string method);
string to_bytes(char* x);
string get_contract_address();
string shell(string _request);

const char* rpc_call(string method, string params, string port);

string build_contract(char* seed);
bool deploy_contract(string contract, string contractName, string port);
//string call_contract(char* method)
bool write_seed(string seed);


#endif
