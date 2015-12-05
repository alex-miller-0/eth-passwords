# eth-passwords
A &#208;app to create and manage cryptographically secure passwords on the ethereum blockchain. This is a stab at a password manager that you can access anywhere from your ethereum account. It still requires a single master password, but can only be used if you unlock your ethereum account so hopefully that's secure. The passwords themselves are hashes, so the idea is that you will use your browser's "Save Password" option and if you switch computers you can recover the passwords from the ethereum blockchain.

## Components
This is a &#208;app for managing passwords, which are themselves SHA3 hashes of your Ethereum address, a master password, and some identifier string (i.e. a clue to what you are making the password for).
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

## Command Line Flags
Use the following flags to manage your passwords:

**--test** uses the private blockchain included in the repo

**-i** initializes a new contract

TODO:
**-s \<seed\>** uses the argument as the seed value

**-sw** writes the seed (requires *-s*) to a local file

**-sd** deletes the seed file

**-p \<password_hash\>** uses the seed (requires *-s* or a seed file) and a given password hash to output the real password

**-m \<method_title\>** calls one of the methods in the deployed contract. 

### TODO:
- Figure out how to get the contract address from a new computer
- Finish all the methods
- Figure out how to concatenate identifiers array in solididy (it needs to return these)
