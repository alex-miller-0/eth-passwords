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
#(geth --genesis ./private_env/genesis.json --datadir ./private_env --rpc --rpcport 2060 --networkid 257291 --unlock primary --nodiscover --maxpeers 0 > ./log.txt 2>&1) &



# Remove line breaks and comments from the contract
#sed '/\/\//d' ./../contract/passwords.sol | tr '\n' ' ' > ./solc_compile/Password.sol
one_liner="$(sed '/\/\//d' ./../contract/passwords.sol | tr '\n' ' ' | grep "")";

# Compile to hex binary
compile='{"jsonrpc":"2.0","method":"eth_compileSolidity", "params":["'${one_liner}'"], "id":1}';
hex=$(curl --silent -X POST --data "${compile}" localhost:2060 | python -c "import json,sys;obj=json.load(sys.stdin);print obj['result']['Password']['code'];");

# Form a string of the bytecode
prefix="0x";
contract="$prefix$hex";

# Get primary account address
primary=$(curl --silent -X POST --data '{"jsonrpc":"2.0.", "method":"eth_accounts", "params":[], "id":1}' localhost:2060 | grep '' | python -c "import json,sys;obj=json.load(sys.stdin);print obj['result'][0];");
echo $primary
# Unlock account
unlock='{"jsonrpc":"2.0", "method":}'

# Deploy the contract
deploy='{"jsonrpc":"2.0","method":"eth_sendTransaction", "params":[{"from":"'${primary}'", "data":"'${contract}'"}], "id":1}'
curl --silent -X POST --data "${deploy}" localhost:2060 | grep ''


# Kill the geth daemon
#pkill geth

