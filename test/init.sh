# 1) Run this to generate a new private blockchain (./private_env should be empty)
# 2) Kill the console and delete everything in private_env except the keystore directory
# 3) Copy your address from the keystore into the genesis alloc object and seed your address with a bunch of ether
# 4) Run this again to generate a new private blockchain

# Followed from this guide: http://adeduke.com/2015/08/how-to-create-a-private-ethereum-chain/

geth --genesis ./genesis.json --datadir ./private_env --networkid 257291 --nodiscover --maxpeers 0 console
