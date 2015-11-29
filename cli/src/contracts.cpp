// Functions for interacting with contracts
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;


//===============================================================================
// HELPER FUNCTIONS
//===============================================================================

// Get the first four bytes of sha3 hash of method header
// These need to be found in the console ahead of time because the RPC method does not work...
string get_first_four(string method){
    if(method=="getSeed"){return "39e7357c";}
    return "0";
}

// Convert a string to 32 bytes (i.e. pad it with leading zeros)
string to_bytes32(char* x){
    stringstream y;
    y << std::setw( 64 ) << std::setfill( '0' ) << x;
    return y.str();
}

// Get the primary address (saved to a file; assumes a contract has been deployed)
string get_contract_address(){
    ifstream infile("./.store/address");
    string line;
    string address;
    while(infile >> line){address+=line;}
    return line;
}

//===============================================================================
// BUILDING THE CONTRACT
//===============================================================================

// Build the Password contract before deploying
string build_contract(char* seed){
    string contract = "contract Password {";
    // Global vars
    contract += "bytes32 Seed="+to_bytes32(seed)+";";
    contract += "bytes32[] public Identifiers;";
    // Functions

    // For testing: get the seed
    contract += "function getSeed() returns(bytes32 seed){return Seed;}";

    // Create an identifier and a password
    //contract += "function create(address _user_address, bytes32 _identifier, bytes32 _seed){ if (sha3(_user_address,_seed) == Seed){Identifiers[Identifiers.length++] = _identifier;returnHash(_user_address, _seed, _identifier);}}";
    
    // Get a password given an identifier and the seed
    //contract += "function get_password(address _user_address, bytes32 _identifier, bytes32 _seed){if (sha3(_user_address, _seed) == Seed){for (uint i=0; i<Identifiers.length; i++){if (Identifiers[i] == _identifier){returnHash(_user_address, _seed, _identifier);}}}}";
    
    // Utility functions for returning data
    //contract += "function returnHash(address _user_address, bytes32 _seed, bytes32 _identifier) returns (bytes32 hash){return sha3(_user_address, _seed, _identifier);}";
    //contract += "function fReturn(string to_return) returns (string) {return to_return;}";
    
    contract += "}";
    
    return contract;
}


//===============================================================================
// INTERACTING WITH THE BLOCK CHAIN
//===============================================================================