#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>

// Functions for dealing with contracts
#include "src/contracts.h"

using namespace std;



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
        // If this is a test, spin up a geth instance
        if(arg== "--test"){
            system("(geth --genesis ./../test/private_env/genesis.json --datadir ./../test/private_chain --rpc --rpcport 2060 --networkid 257291 --unlock primary --nodiscover --mine --minerthreads 8 --maxpeers 2 --unlock 0 --password ./../test/private_env/password.txt > ./../test/log.txt 2>&1) &");
            system("sleep 1");
        }

        // Get the method header
        if(arg == "-m"){
            cout << "Calling method... \n";
            call_contract(argv[i+1]);
        }

        // Create and initialize the contract
        if(arg == "-i"){
            cout << "\n\nInitializing new contract... \n" << flush;
            string contract = build_contract(argv[i+1]);
            try { bool deployed = deploy_contract(contract, "Password", "2060"); }
            catch (...) {cout << "caught an exception";}
        }
    }
    // Kill geth
    system("pkill geth");
    
    // Build the contract using a seed
    //string x = build_contract("42");
    //cout << x;

    // Deploy the contract and initialize it with a seed
    //system("./init.sh");
    

    // Interact with the contract

    return 0;
}
