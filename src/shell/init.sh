input_contract=$1;

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

(geth --genesis ./../test/private_env/genesis.json --datadir ./../test/private_chain --rpc --rpcport 2060 --networkid 257291 --unlock primary --nodiscover --mine --minerthreads 8 --maxpeers 2 --unlock 0 --password ./../test/private_env/password.txt > ./log.txt 2>&1) &

# Give geth a few seconds to boot up
sleep 5

# Remove line breaks and comments from the contract
#one_liner="$(sed '/\/\//d' ./../contract/passwords.sol | tr '\n' ' ' | grep "")";

# Compile to hex binary
compile='{"jsonrpc":"2.0","method":"eth_compileSolidity", "params":["'${input_contract}'"], "id":1}';

contract_info=$(curl --silent -X POST --data "${compile}" localhost:2060)

contract=$(echo $contract_info | python -c "import json,sys;obj=json.load(sys.stdin);print obj['result']['Password']['code'];");

# Get primary account address
primary=$(curl --silent -X POST --data '{"jsonrpc":"2.0.", "method":"eth_accounts", "params":[], "id":2}' localhost:2060 | grep '' | python -c "import json,sys;obj=json.load(sys.stdin);print obj['result'][0];");

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
fi

# Kill geth and exit
pkill geth
exit
