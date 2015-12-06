// Functions for creating and managing passwords locally
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>

// Include openssl for local encryption
#include "openssl/include/openssl/aes.h"

using namespace std;



// Set a max password size to use with encryption functions
static int MAX_PASSWORD_SIZE = 16;



//===============================================================================
// HELPER FUNCTIONS
//===============================================================================


// Pad a string with trailing spaces to reach desired length
string pad_string(string input, int size){
	if (input.size() >= size) {return input; }
	else {
		while (input.size() < size) { input += " "; }
		return input;
	}
}


// Encrypt a password with AES (TODO move to better encryption algorithm)
// Key must be 
// Password must be <= 16 characters
void encrypt_password(string password, string seed, unsigned char output[]){
	
	// Password must be <= 16 characters
	int size = MAX_PASSWORD_SIZE;
	// If the password is too large, return an error
	if ( password.size() > size ){exit;}
	
	// If the password is too small, pad it with spaces
	else if ( password.size() < size ){ password = pad_string(password, size); }

	// Copy seed to unsigned char array
	const char* c_seed = seed.c_str();
	unsigned char KEY[size];
	strncpy( (char*) KEY, c_seed ,size);

	// Copy password to unsigned char array
	const char* c_password = password.c_str();
	unsigned char PASSWORD[size];
	strncpy( (char*) PASSWORD, c_password, size);

	// Set the buffer
    unsigned char enc_out[1024];

    // Instantiate the key
    AES_KEY enc_key, dec_key;

    // Encrypt the password with the key (a.k.a seed)
    AES_set_encrypt_key(KEY, 256, &enc_key);
    AES_encrypt(PASSWORD, enc_out, &enc_key);      

    // De-pointer enc_out and copy it to the output
    int size_of_enc = sizeof(enc_out);
    unsigned char encrypted_password[1024];
    for (int i=0; *(enc_out+i); i++){
    	encrypted_password[i] = *(enc_out+i);
    }
    printf("encrypted: %s\n", encrypted_password);

    // Copy the encrypted password to the output variable
    output = encrypted_password;
}


// Decrypt the password using a complementary algorithm (also AES for now)
string decrypt_password(unsigned char encrypted_password[], string seed){
	
	// Copy seed to unsigned char array
	const char* c_seed = seed.c_str();
	unsigned char KEY[MAX_PASSWORD_SIZE];
	strncpy( (char*) KEY, c_seed ,MAX_PASSWORD_SIZE);

	// Instantiate the key
	AES_KEY dec_key;

	// Set the buffer
	unsigned char dec_out[1024];

	// Test decryption
    AES_set_decrypt_key(KEY, 256, &dec_key);
    AES_decrypt(encrypted_password, dec_out, &dec_key);
    //AES_decrypt(enc_out, dec_out, &dec_key);
    
    printf("decrypted: %s\n\n", dec_out);
    return "test";
}




//===============================================================================
// MANAGE LOCAL FILES (All files are stored in ./.store)
//===============================================================================


// If the user wants a permanent seed, write this file. It can be deleted at any time and any seed can be
// overwritten (using -s) when a password is generated.
bool write_seed(string seed){
	ofstream f("./.store/seed");
	f << seed;
	f.close();
	return 1;
}


// Destroy a local seed file
bool destroy_seed(){
	system("rm ./.store/seed");
	return 1;
}


// Get the seed from a local file
string get_seed(){
	// Check if the file exists
	ifstream infile("./.store/seed");
    if (infile.good()){
    	ifstream infile("./.store/seed");
    	string line; string seed;
    	while(infile >> line){ seed += line; }
    	return seed;
    }
    else { return ""; }
}


// Write an encrypted password to a file given a hint
bool write_password(string password, string seed, string hint){
	
	// Encrypt the password into an unsigned char array
	unsigned char encrypted[16];
	encrypt_password("shitstorm", "seed", encrypted);

	// Convert unsigned char array into hex
	stringstream s;
    s << encrypted;
    string encrypted_password = s.str();
    cout << "\n\nTEST PASS: " << encrypted_password << "\n\n";


    decrypt_password(encrypted, "seed");

	// TO GO IN READ PASSWORD FUNCTION 

	// Read the password from a file in hex
    /*string filepath = "./.store/" + seed;
	ifstream in(filepath.c_str());
	string line; string x;
	while (in >> line) { x += line; }

	cout << "Reading from file: " << x << "\n\n";
	unsigned char test[16];
	for (int i=0; i < x.size(); i++){
		test[i] << x[i];
		cout << test[i] << "\n"; 
	}
	cout << "test: " << test << "\n";
	*/
	//ofstream f("./.store/passwords/%s", hint);
	//int x = sizeof(encrypted_password);
	//cout << x;



	return 1;
}

