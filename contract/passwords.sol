contract Password {
    
    // In the cli, the address is prepopulated with the user's wallet's primary address.
    address MyAddress; 
    
    // A simple struct containing a password/hint combination
    struct Object {bytes32 hint; bytes32 password;} 
    
    // The bank will be an array of password/hint combinations
    Object[0] bank; 
    
    // To add a password (an encrypted string), simply append the bank
    function addPassword(bytes32 pass, bytes32 hint) { 
        bank[bank.length+1].password = pass; 
        bank[bank.length+1].hint = hint; 
    } 
    
    // Search the bank for a hint matching the one that was passed. If it gets a hit, it returns the encrypted password.
    function getPassword(bytes32 hint) returns (bytes32) { 
        bytes32 to_return; 
        for (uint i=0; i<bank.length; i++) {
            if (bank[i].hint == hint) { 
                to_return = bank[i].password; 
            }
        } 
        return to_return; 
    }
    
}
