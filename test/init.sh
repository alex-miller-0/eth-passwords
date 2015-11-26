#debug
#set -vx

# Deploy the contract on a private blockchain for testing
########################################################

# Spin up the private blockchain as a daemon (daemon syntax from bash script = (process &) & )
# ----
# For first time use:
# 1) Run this to generate a new private blockchain (./private_env should be empty)
# 2) Kill the console and delete everything in private_env except the keystore directory
# 3) Copy your address from the keystore into the genesis alloc object and seed your address with a bunch of ether
# 4) Run this again to generate a new private blockchain
# Followed from this guide: http://adeduke.com/2015/08/how-to-create-a-private-ethereum-chain/
# ----
# For subsequent use, this command is all you need
(geth --genesis ./private_env/genesis.json --datadir ./private_chain --rpc --rpcport 2060 --networkid 257291 --unlock primary --nodiscover --mine --minerthreads 8 --maxpeers 2 --unlock 0 --password ./private_env/password.txt > ./log.txt 2>&1) &
# Give geth a few seconds to boot up
printf "\nTESTING CONTRACT DEPLOYMENT\n============================\n\n"
echo "Booting private blockchain..."
sleep 5


# Remove line breaks and comments from the contract
one_liner="$(sed '/\/\//d' ./../contract/passwords.sol | tr '\n' ' ' | grep "")";

# Compile to hex binary
compile='{"jsonrpc":"2.0","method":"eth_compileSolidity", "params":["'${one_liner}'"], "id":1}';
hex=$(curl --silent -X POST --data "${compile}" localhost:2060 | python -c "import json,sys;obj=json.load(sys.stdin);print obj['result']['Password']['code'];");

# Form a string of the bytecode
prefix="0x";
contract="$prefix$hex";

# Get primary account address
primary=$(curl --silent -X POST --data '{"jsonrpc":"2.0.", "method":"eth_accounts", "params":[], "id":2}' localhost:2060 | grep '' | python -c "import json,sys;obj=json.load(sys.stdin);print obj['result'][0];");
printf "Primary account: ${primary}\n"

# Deploy the contract (and get the txn hash

# Override defaults for gas and gas price to higher values (I think these are high enough...)
# Note; therse are encoded with RLP (using pyrlp package); check out http://vitalik.ca/ethereum/rlp.html
# TODO roll an RLP method in C
gas="8301[a8"
gasprice="9184e72a000"

deploy='{"jsonrpc":"2.0","method":"eth_sendTransaction", "params":[{"from":"'${primary}'", "data":"'${contract}'","gas":"0x'${gas}'","gasPrice":"0x'${gasprice}'"}], "id":3}'
contract_txn=$(curl --silent -X POST --data "${deploy}" localhost:2060 |  python -c "import json,sys;obj=json.load(sys.stdin);print obj['result']")
echo "Deployed contract..."
echo "Mining for a few seconds..."
sleep 10

# Get the address of the contract
get_address='{"jsonrpc":"2.0","method":"eth_getTransactionReceipt","params":["'${contract_txn}'"], "id":4}'
contract_receipt=$(curl --silent -X POST --data "${get_address}" localhost:2060 | grep '')
echo $contract_receipt
contract_address=$(echo $contract_receipt | python -c "import json,sys; obj=json.load(sys.stdin);print obj['result']['contractAddress'];");
block_no=$(echo $contract_receipt | python -c "import json,sys; obj=json.load(sys.stdin);print obj['result']['blockNumber'];");
printf "Contract Address: ${contract_address} \n"
printf "Contract Block Number: ${block_no} \n"

# Make sure the contract is deployed (i.e. is not "0x")
check_deployed='{"jsonrpc":"2.0", "method":"eth_getCode", "params":["'${contract_address}'", "'${block_no}'"], "id":5}'
deployed_code=$(curl --silent -X POST --data "${check_deployed}" localhost:2060 | python -c "import json,sys;obj=json.load(sys.stdin);print obj['result']")
printf "Deployed code: ${deployed_code} \n"


# Test calling the contract
# 1) Get the first 4 bytes of the SHA3 hash of the function definition
test="test(uint32 x)";
get_method='{"jsonrpc":"2.0", "method":"web3_sha3", "params":["'${test}'"], "id":6}'
method_id=$(curl --silent -X POST --data "${get_method}" localhost:2060 | python -c "import json,sys;obj=json.load(sys.stdin);print obj['result']");
# The output is a hash, but it is represented as a string. Parse it with xxd and then reconvert only the first 4 bytes to hex with xxd.
first_four=$(printf "\x${method_id}" | xxd -r -p | xxd -l 4 -p)
#echo $first_four

# 2) Append the hex values of the parameters you are passing to the hex generated in 1)
# Just use the number 7 here. Figure out how to pad it into a 32 bit int later
var="0000000000000000000000000000000000000000000000000000000000000007"

# 3) Use RPC eth_call with the above hex as the "data" param
echo "RPC eth_call to method ${test}..."
contract_call="0x${first_four}${var}"
call_contract='{"jsonrpc":"2.0", "method":"eth_call", "params":[{"to":"'${contract_address}'", "from":"'${primary}'", "data":"'${contract_call}'"}, "latest"], "id":7}'
call_return=$(curl -X POST --data "${call_contract}" localhost:2060) #| python -c "import json,sys;obj=json.load(sys.stdin);print obj['result']");
#echo $call_contract
echo $call_return
#printf "\n\n"


# Kill the geth daemon
pkill geth

