# eth-passwords
#####A &#208;app to create and manage cryptographically secure passwords on the ethereum blockchain.
I'm really tired of trying to remember my passwords, reusing passwords, and generally being frightened of having my browser save my passwords (for fear of being on a new computer and getting stuck). Passwords suck, so this is a stab at making them suck less.

## Components
This is a &#208;app for managing passwords, which are themselves SHA3 hashes of your Ethereum address, some identifier string (i.e. a clue to what you are making the password for), and a master password.
##### Contract
The contract is written in Solidity and will be called to do any of the following: initialize the contract with your address and master password, create a new password, retreive a password, delete a password, see all identifiers.
##### Test Environment
This is a private blockchain which has been initialized in the repo. An address has been seeded with a bunch of fake ether using a trick that modifies the genesis block. This private chain can be used to test the contract before deploying to the real blockchain and using real ether.
##### Command Line Tool
This is what you use to interact with the contract. This whole part is TODO, but I will probably start by using RPC to execute geth/eth commands directly. Thus, you will need to have eth/geth running before using this.


## Workflow
The general idea (in progress) is this:

1. Make sure you have an ethereum account. Deploy a new contract with your account using a command line tool.
2. Initialize the contract with your account and some root password. Don't forget the latter or you'll be in trouble.
3. Call the contract to create a new password. You will need to pass a) your address, b) some string to identify what the password is used for, c) your root password

Once these steps are carried out, you have a few more RESTful options.

* Get a list of all places you have passwords stored for (i.e. the strings you used to identify them when you initialized)
* Get a specific password given the above identity and your root password
* Delete a speciffic password given the above identity and your root password

### TODO:
- Write a shell script to autocompile
- Figure out how to concatenate identifiers array
- Script to spin up new instance of contract (for a new account)
- init function (initialize a new contract)
- test stuff?
