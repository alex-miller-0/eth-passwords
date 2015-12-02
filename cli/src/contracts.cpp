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
const char* rpc_call(string method, string params, string port){
    string json="\'{\"jsonrpc\":\"2.0\",\"method\":\""+method+"\", \"params\":[\""+params+"\"], \"id\":1}\'";
    
    // Get the first four bytes of the method header (static string)
    string first_four = get_first_four(method);    
    string vars = "";
    string contract_address = get_contract_address();

    // Build the request
    string request = "curl --silent -X POST --data " +json+ " localhost:"+port;
    cout << request + "\n\n";

    string _response = shell(request);
    const char* response = _response.c_str();
    return response;
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
// DEPLOYING THE CONTRACT
//===============================================================================
string deploy_contract(string contract, string contractName, string port){
    // Compile the contract
    const char* compiled_contract = rpc_call("eth_compileSolidity", contract, port);
    cout << "Compiled contract: \n";
    cout << compiled_contract;
    cout << "\n\n";
    
    // Parse the json
    rapidjson::Document d;
    d.Parse(compiled_contract);
    rapidjson::Value& s = d["result"];
    string code = s["Password"]["code"].GetString();
    cout << code;

    // Get primary account address
//primary=$(curl --silent -X POST --data '{"jsonrpc":"2.0.", "method":"eth_accounts", "params":[], "id":2}' localhost:2060 | grep '' | python -c "import json,sys;obj=json.load(sys.stdin);print obj['result'][0];");
/*
    # Deploy the contract (and get the txn hash

# Override defaults for gas and gas price to higher values (I think these are high enough...)
# Note; therse are encoded with RLP (using pyrlp package); check out http://vitalik.ca/ethereum/rlp.html
# TODO roll an RLP method in C
gas='100000'
gasprice='10000000000000'

# Send the transaction with the contract as the data
deploy='{"jsonrpc":"2.0","method":"eth_sendTransaction", "params":[{"from":"'${primary}'", "data":"'${contract}'","gas":"0x'${gas}'","gasPrice":"0x'${gasprice}'"}], "id":3}'
contract_txn=$(curl --silent -X POST --data "${deploy}" localhost:2060 |  python -c "import json,sys;obj=json.load(sys.stdin);print obj['result']")

sleep 8

# Get the address of the contract from the transaction receipt
get_address='{"jsonrpc":"2.0","method":"eth_getTransactionReceipt","params":["'${contract_txn}'"], "id":4}'
contract_receipt=$(curl --silent -X POST --data "${get_address}" localhost:2060 | grep '')
contract_address=$(echo $contract_receipt | python -c "import json,sys; obj=json.load(sys.stdin);print obj['result']['contractAddress'];");
block_no=$(echo $contract_receipt | python -c "import json,sys; obj=json.load(sys.stdin);print obj['result']['blockNumber'];");

# Make sure the contract is deployed (i.e. is not "0x")
check_deployed='{"jsonrpc":"2.0", "method":"eth_getCode", "params":["'${contract_address}'", "'${block_no}'"], "id":5}'
deployed_code=$(curl --silent -X POST --data "${check_deployed}" localhost:2060 | python -c "import json,sys;obj=json.load(sys.stdin);print obj['result']")
if [ ${#deployed_code} > 2 ]; then
    echo $contract_address
else
    echo 0
fi*/

     return "";
}

