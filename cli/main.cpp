#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>

#include "src/contracts.h"
using namespace std;



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
