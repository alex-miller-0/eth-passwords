// Functions for interacting with contracts
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>

// JSON parser (https://github.com/miloyip/rapidjson)
#include "rapidjson/document.h"

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

// Make a shell request and return the output
string shell(string _request){
    // Convert to char array
    const char* request = _request.c_str();
    // Execute shell script
    FILE *fp = popen(request, "r");
    char buf[1024];
    string response;
    while(fgets(buf, 1024, fp)) {response += buf;}
    return response;
}

//===============================================================================
// INTERACTING WITH THE BLOCK CHAIN
//===============================================================================

// Make an rpc call to the blockchain that is currently running
rapidjson::Value& rpc_call(string method, string params, string port){
    string json="\'{\"jsonrpc\":\"2.0\",\"method\":\""+method+"\", \"params\":["+params+"], \"id\":1}\'";
    
    // Get the first four bytes of the method header (static string)
    string first_four = get_first_four(method);    
    string vars = "";
    string contract_address = get_contract_address();

    // Build the request
    string request = "curl --silent -X POST --data " +json+ " localhost:"+port;
    string _response = shell(request);
    const char* response = _response.c_str();

    // Instantiate JSON parser
    rapidjson::Document d;
    d.Parse(response);
    
    // Assert that d is an object
    // TODO throw an exception so this doesn't crash the whole process
    assert (d.IsObject());

    rapidjson::Value& return_response = d["result"];

    return return_response;
}

//===============================================================================
// BUILDING THE CONTRACT
//===============================================================================

// Build the Password contract before deploying
string build_contract(char* seed){
    string contract = "\"contract Password {";
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
    
    contract += "}\"";
    
    return contract;
}

//===============================================================================
// DEPLOYING THE CONTRACT
//===============================================================================
// Redirect output (i.e. address of contract) to a file
bool write_contract_address(string address){
    try {
        ofstream f("./.store/address");
        f << address;
        f.close();
        return 1;
    } 
    catch (int e) {return 0;}
}


bool deploy_contract(string contract, string contractName, string port){

    // Compile the contract
    rapidjson::Value& compiled_contract = rpc_call("eth_compileSolidity", contract, port);
    string code = compiled_contract["Password"]["code"].GetString();

    // Get primary account address
    rapidjson::Value& primary_address = rpc_call("eth_accounts", "", port);
    string address = primary_address[0].GetString();
    
    // Deploy the contract and get the txn hash
    string deploy_data = "{\"from\":\""+address+"\", \"data\":\""+code+"\"}";
    rapidjson::Value& deploy = rpc_call("eth_sendTransaction", deploy_data, port);
    string txn = deploy.GetString();

    // Wait 10 seconds for the contract to deploy (may need to change this to ~15 for live blockchain)
    // TODO This fails sometimes (i.e. returns null and crashes rapidjson); throw an exception when that happens
    cout << "Deploying contract ";
    for (int i=0; i<10; i++){
        system("sleep 1");
        cout << "=" << flush;
    }
    cout << "\n";

    // Get the address of the contract from the transaction receipt
    rapidjson::Value& txn_receipt = rpc_call("eth_getTransactionReceipt", "\""+txn+"\"", port);
    string txn_address = txn_receipt["contractAddress"].GetString();
    string block_no = txn_receipt["blockNumber"].GetString();
    ;
    // Make sure the code was successfully deployed
    rapidjson::Value& check_deployed = rpc_call("eth_getCode", "\""+txn_address+"\", \""+block_no+"\"", port);
    string deployed = check_deployed.GetString();

    // If the code did not deploy, this will have returned "0x". Otherwise, it should return the full EVM code.
    int len = deployed.size();
    if (len > 2){
        // Write the address to a file
        bool written = write_contract_address(deployed);
        if (written == 1){cout << "Contract deployed successfully!\n\n" << flush; return 1;}
        else {return 0;}
    }
    else {
        cout << "Contract deployment FAILED.\n" << flush;
        return 0;
    }

}

/*
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
*/
