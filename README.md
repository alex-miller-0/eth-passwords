# eth-passwords
A way to create and manage cryptographically secure passwords on the ethereum blockchain

I have not written all of the components yet, but loosely here is the idea:

1) Make sure you have an ethereum account. Deploy a new contract with your account using a command line tool (TODO).
2) Initialize the contract with your account and some root password. Don't forget the latter or you'll be in trouble.
3) Call the contract to create a new password. You will need to pass a) your address, b) some string to identify what the password is used for, c) your root password

Once these steps are carried out, you have a few more RESTful options.

A) Get a list of all places you have passwords stored for (i.e. the strings you used to identify them when you initialized)
B) Get a specific password given the above identity and your root password
C) Delete a speciffic password given the above identity and your root password

TODO:
- Write a shell script to autocompile
- Figure out how to concatenate identifiers array
- Script to spin up new instance of contract (for a new account)
- init function (initialize a new contract)
- test stuff?
