#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;


// Convert a string into 32 bytes
string to_bytes32(char* x){
    stringstream y;
    y << std::setw( 64 ) << std::setfill( '0' ) << x;
    return y.str();
}

// Get the first four bytes of sha3 hash of method header
// These need to be found in the console ahead of time because the RPC method does not work...
string get_first_four(string method){
    if(method=="getSeed"){return "39e7357c";}
    return "0";
}

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

// Deploy a contract to the block chain
int deploy_contract(char* arg){
    // Build the contract and use it as an argument to the bash init script
    string shell_init = "shell/init.sh \"" + build_contract(arg) + "\"";       
    const char* shell_init_command = shell_init.c_str();

    // Redirect output (i.e. address of contract) to a file
    FILE *fp = popen(shell_init_command, "r");
    char buf[1024];
    ofstream faddress;
    faddress.open ("./.store/address");
    while (fgets(buf, 1024, fp)) {faddress << buf;}
    faddress.close();
    fclose(fp);

    cout << "Done.\n";
}

// Get the primary address (saved to a file; assumes a contract has been deployed)
string get_contract_address(){
    ifstream infile("./.store/address");
    string line;
    string address;
    while(infile >> line){address+=line;}
    return line;
}

string call_contract(char* method){
    // Get the first four bytes of the method header
    string first_four = get_first_four(method);    
    string vars = "";
    string contract_address = get_contract_address();

    // Build the command
    string shell_call = "shell/call.sh \"0x"+first_four+vars+"\" \""+contract_address+"\"";
    const char* shell_call_command = shell_call.c_str();

    // Execute shell script
    FILE *fp = popen(shell_call_command, "r");
    char buf[1024];
    ofstream fcall;
    fcall.open("./.store/getSeed");
    while(fgets(buf,1024, fp)) {cout << buf;}
    fcall.close();
    fclose(fp);

    return "";
}



int main(int argc, char* argv[]) {
    // Parse command line flags
    string method_id = "0x";
    for (int i=1; i<argc; i++){
        string arg = argv[i];
        // Get the method header
        if(arg == "-m"){
            cout << "Calling method... \n";
            call_contract(argv[i+1]);
        }

        // Create and initialize the contract
        if(arg == "-i"){
            cout << "Initializing new contract... ";
            deploy_contract(argv[i+1]);
        }

    }
    
    // Build the contract using a seed
    //string x = build_contract("42");
    //cout << x;

    // Deploy the contract and initialize it with a seed
    //system("./init.sh");
    

    // Interact with the contract

    return 0;
}
