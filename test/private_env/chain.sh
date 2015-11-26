# Debug stuff on the private chain using the console

geth --genesis ./genesis.json --datadir ./../private_chain --rpc --rpcport 2060 --networkid 257291 --unlock primary --nodiscover --maxpeers 0 --unlock 0 --password ./password.txt console

