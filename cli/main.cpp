#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>

// Functions for dealing with contracts
#include "src/contracts.h"
// Functions for managing passwords locally
#include "src/passwords.h"

using namespace std;


//===============================================================================
// HELPER FUNCTIONS
//===============================================================================

// Look for a specific flag in a loop of all arguments. I need to look for flags independent of their order. Return the next number.
int check_flag(char* args[], int size, string flag){
    for (int i=1; i < size; i++){ if (args[i] == flag) {return i+1;} }
    return 0;
}

// Start geth as a private block chain
bool start_private_block_chain(){
    system("(geth --genesis ./../test/private_env/genesis.json --datadir ./../test/private_chain --rpc --rpcport 2060 --networkid 257291 --unlock primary --nodiscover --mine --minerthreads 8 --maxpeers 2 --unlock 0 --password ./../test/private_env/password.txt > ./../test/log.txt 2>&1) &");
    system("sleep 1");
    return 1;
}


//===============================================================================
// MAIN FUNCTION LOGIC
//===============================================================================

int main(int argc, char* argv[]) {
    
    // DECLARE GLOBAL VARS
    //==============================
    string SEED = "";
    string HINT = "";
    string PASSWORD = "";



    // PARSE COMMAND LINE FLAGS
    //==============================

    // Spin up a private blockchain
    int is_test = check_flag(argv, argc, "--test");
    if (is_test > 0) {start_private_block_chain();}

    // Initialize (deploy) a new contract
    int init_i = check_flag(argv, argc, "-i");

    // Call a method from the block chain
    int method_i = check_flag(argv, argc, "-m");

    // Use the provided seed value
    int seed_i = check_flag(argv, argc, "-s");

    // Write the seed value to a file
    int seed_write = check_flag(argv, argc, "-sw");
    
    // Destroy the seed file
    int seed_destroy = check_flag(argv, argc, "-sd");

    // Assign a password if one is provided
    int password_i = check_flag(argv, argc, "-p");

    // Encrypt?
    int encrypt = check_flag(argv, argc, "--encrypt");

    // Decrypt?
    int decrypt = check_flag(argv, argc, "--decrypt");

    // EXECUTE:
    //==============================

    // Use the provided seed if available. Otherwise for a seed file.
    // This will return an empty string if no seed is provided and no file exists.
    if (seed_i > 0) { SEED = argv[seed_i]; }
    else { SEED = get_seed(); }

    // Destroy the existing seed file
    if (seed_destroy > 0){ 
        destroy_seed(); 
    }

    // Write the seed value to a file
    if (seed_i > 0 && seed_write > 0){
        write_seed(SEED);
    }

    // Initialize a contract
    if (init_i > 0) {
        string primary_address = get_primary_address("2060");
        string contract = build_contract(primary_address);
        cout << "Contract: " << contract << endl;
        bool deployed = deploy_contract(contract, "Password", "2060");
    } 

    //else if (method_i > 0){
        //string method_id = "0x";
        //call_contract(argv[i+1]);
    //}

    // Set the hint based on encrypt or decrypt flags
    if (encrypt > 0){ HINT = argv[encrypt]; } 
    else if (decrypt > 0) { HINT = argv[decrypt]; }

    // Set the password
    if (password_i > 0){ PASSWORD = argv[password_i]; }

    // Encrypt a password. Make sure seed, hint, and password are available.
    if (encrypt > 0 && (seed_i > 0 || SEED != "") && HINT != "" && PASSWORD != ""){
        
        encrypt_and_write_password(PASSWORD, SEED, HINT);
    }

    // Decrypt a password. Make sure both seed and hint are available.
    if (decrypt > 0 && (seed_i > 0 || SEED != "") && HINT != "" ){
        decrypt_and_read_password(SEED, HINT);
    }

    cout << "PASSWORD: " << PASSWORD << endl;
    cout << "SEED: " << SEED << endl;
    cout << "HINT: " << HINT << endl;

    // Keep this for testing. The command line flags don't work yet
    //encrypt_and_write_password("passwordpassword", "helloworlddd", "hint");
    //decrypt_and_read_password("helloworlddd", "hint");

    // Finalize and exit
    system("pkill geth");
    return 1;
    
}
