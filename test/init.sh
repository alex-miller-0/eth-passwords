#debug
#set -vx

# Deploy the contract on a private blockchain for testing

# Spin up the private blockchain as a daemon 
# ----
# For first time use:
# 1) Run this to generate a new private blockchain (./private_env should be empty)
# 2) Kill the console and delete everything in private_env except the keystore directory
# 3) Copy your address from the keystore into the genesis alloc object and seed your address with a bunch of ether
# 4) Run this again to generate a new private blockchain
# Followed from this guide: http://adeduke.com/2015/08/how-to-create-a-private-ethereum-chain/
# ----
# For subsequent use, this command is all you need
#geth --genesis ./genesis.json --datadir ./private_env --rpc --rpcport 2060 --networkid 257291 --nodiscover --maxpeers 0 > ./log.txt 2>&1

# Remove line breaks and comments from the contract
#sed '/\/\//d' ./../contract/passwords.sol | tr '\n' ' ' > ./solc_compile/Password.sol
contract="$(sed '/\/\//d' ./../contract/passwords.sol | tr '\n' ' ' | grep "")";

# Compile to hex binary
#solc --bin -o ./solc_compile ./solc_compile/Password.sol
hmm='{"jsonrpc":"2.0","method":"eth_compileSolidity", "params":["'${contract}'"], "id":1}'
echo $hmm
curl --silent -X POST --data $hmm localhost:2060


# Form a string of the bytecode
#prefix="0x";
#bytecode=$(cat ./solc_compile/Password.bin | grep "");
#contract="$prefix$bytecode";

# Get primary address
primary=$(curl --silent -X POST --data '{"jsonrpc":"2.0.", "method":"eth_accounts", "params":[], "id":1}' localhost:2060 |  python -c "import json,sys;obj=json.load(sys.stdin);print obj['result'][0];");
echo $primary

# Deploy the contract
curl -X POST --data '{"jsonrpc":"2.0", "method":"eth_sendTransaction", "params":["from":"'"$primary"'", "data":"'"$contract"'"], "id":1}' localhost:2060 


# Kill the geth daemon
#pkill geth

