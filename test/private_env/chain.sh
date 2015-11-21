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

geth --genesis ./genesis.json --datadir ./../private_chain --rpc --rpcport 2060 --networkid 257291  --nodiscover --maxpeers 0 --unlock 0 --password ./password.txt  > ./../private_chain/log.txt 2>&1


