# eth-passwords
This is a &#208;app to create and manage cryptographically secure passwords on the ethereum blockchain. Deploy a contract and start adding passwords (SHA3 hashes) to the blockchain (along with hints so that you know which password is which). A master password (seed) is used locally to transform the password hashes (stored in the blockchain) into the true passwords you will use. You may keep this seed locally or just remember it.


## Command Line Flags
Use the following flags to manage your passwords:

    Environments:
    --test          Uses the private blockchain included in the repo
    
    Actions:
     -i             Initializes and deploys a new contract. 
                    Requires geth to be running with RPC enabled and an unlocked Ethereum account.
    
    Seed storage:
    -s <arg>        Uses the argument as the seed value
    -sw             Writes the seed (requires -s) to a local file
    -sd             Deletes the seed file
   

    TODO
    -p <arg>        Uses the seed (arg) and a given password hash to output the real password

    -m <arg>        Calls one of the methods in the deployed contract (arg = method title).
## Components
There are three main components in this repo.
#### Contract
The contract is written in Solidity and will be called to do any of the following: deploy a new contract (i.e. a password "wallet"), create a new password, retreive a password, delete a password, see all identifiers.
#### Test Environment
This is a private blockchain with RPC enabled. An address has been seeded with a bunch of fake ether using a trick that modifies the genesis block. This private chain can be used to test the contract before deploying to the real blockchain and using real ether.
#### Command Line Tool
This is what you use to interact with the contract.


## Workflow
The general idea (in progress) is this:

1. Make sure you have an ethereum account. Spin up a geth instance with RPC enabled and unlock your account.
2. Deploy a new contract. The address will be written to a local file.
3. Use the CLI to generate a new password hash with a string serving as some clue to you about what the password will be used for. The CLI will upload the password hash to the block chain and will save it locally too.

At this point, you may use the CLI to view all of your password hashes. To get the *real* password, the CLI will combine the password hash of your choosing with your seed.

You also have a few more RESTful options that utilize the block chain:
* Get a list of all places you have passwords stored for (i.e. the strings you used to identify them when you initialized)
* Get a specific password hash
* Delete a speciffic password hash


### TODO:
- Figure out how to get the contract address from a new computer
- Finish all the methods
- Figure out how to concatenate identifiers array in solididy (it needs to return these)
