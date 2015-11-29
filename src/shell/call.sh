# Call a contract that has been deployed
# First four bytes of method header + variables to use in the call
data=$1
# Address of deployed contract
contract_address=$2

(geth --genesis ./../test/private_env/genesis.json --datadir ./../test/private_chain --rpc --rpcport 2060 --networkid 257291 --unlock primary --nodiscover --mine --minerthreads 8 --maxpeers 2 --unlock 0 --password ./../test/private_env/password.txt > ./log.txt 2>&1) &

# Give geth a few seconds to boot up
sleep 2

# Call the contract
call_contract='{"jsonrpc":"2.0", "method":"eth_call", "params":[{"to":"'${contract_address}'", "from":"'${primary}'", "data":"'${data}'"}], "id":7}'

call_return=$(curl --silent -X POST --data "${call_contract}" localhost:2060 | python -c "import json,sys;obj=json.load(sys.stdin);print obj['result']");
echo $call_return

# Kill the geth daemon
pkill geth



