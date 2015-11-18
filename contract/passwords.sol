contract Password {
    // A seed to hash a given password with
    // This should itself be a hash of the account address and a master password string
    bytes32 Seed;

    // An array of identifier strings to use when getting a password
    bytes32[] public Identifiers;

    // Add an identifier to the array  
    function create(address _user_address, bytes32 _identifier, bytes32 _seed){
        if (sha3(_user_address,_seed) == Seed){
            // Append the Identifiers array with a new one
            Identifiers[Identifiers.length++] = _identifier;
            // Return the newly minted password
            returnHash(_user_address, _seed, _identifier);
        }
    }
    
    // Get a password after finding the identifier in the array
    function get_password(address _user_address, bytes32 _identifier, bytes32 _seed){
        if (sha3(_user_address, _seed) == Seed){
            // Find the appropriate identifier
            for (uint i=0; i<Identifiers.length; i++){
                if (Identifiers[i] == _identifier){
                    returnHash(_user_address, _seed, _identifier);
                }
            }
        }
    }
    
    // Get a list of your identifiers
    // BUZZKILL... solidity does not yet support return of arrays... will need to find a way to concatenate
    //function get_list(address_user_address, bytes32 _seed) returns (bytes32[]){
    //    if (sha3(_user_address,_seed) == Seed){
    //        return Identifiers;
    //    }  
    //}


    // Update an identifier (TODO)

    // Delete an identifier (TODO)
    

    // Return a hash
    function returnHash(address _user_address, bytes32 _seed, bytes32 _identifier) returns (bytes32 hash){
        return sha3(_user_address, _seed, _identifier);
    }


    // Return something (an identifier or password)
    function fReturn(string to_return) returns (string) {
        return to_return;
    }





}
