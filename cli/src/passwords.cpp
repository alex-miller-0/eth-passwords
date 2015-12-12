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
//#include "aes.h"
#include "/usr/local/ssl/include/openssl/aes.h"
using namespace std;



// Set a max password size to use with encryption functions
static int MAX_SEED_SIZE = 16;
// Set a buffer size for encryption
static int BUFFER_SIZE = 1024;


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

// Check that the seed is long enough
string check_seed(string seed){
	
	// If the seed is too large, exit the program
	if ( seed.size() > MAX_SEED_SIZE ){
		cout << "\nYour seed must be < 17 characters";
		exit;
	}
	
	// If the password is too small, pad it with spaces
	else if ( seed.size() < MAX_SEED_SIZE ){ 
		seed = pad_string(seed, MAX_SEED_SIZE); 
	}

	return seed;
}


// Convert a string to a char array
void convert_string(string str, int size, unsigned char to_convert[]){
	
	const char* char_str= str.c_str();
	strncpy( (char*) to_convert, char_str, size);

} 


// Decrypt the password using an algorithm that is complementary to the encryption
string decrypt_password(unsigned char encrypted_password[], string seed){
	
	// Allocate the output buffer
	unsigned char out_buffer[BUFFER_SIZE];


	// Check the seed size
	seed = check_seed(seed);


	// Copy seed string to char array;
	unsigned char key[seed.size()];


	// Convert the seed to a char array and pass it to a oointer
	memcpy( (char*) key, seed.c_str(), seed.size());


	// Set AES key
	AES_KEY dec_key; 


	// Decrypt
	AES_set_decrypt_key(key, 128, &dec_key);
	AES_decrypt(encrypted_password, out_buffer, &dec_key);
	printf("outbuffer: %s\n",out_buffer);


    return "test";
}


// Encrypt a password with AES (TODO move to better encryption algorithm)
// Key must be 16 characters
void encrypt_password(string password, string seed, unsigned char output[]){

	// Check seed size
	seed = check_seed(seed);
	

	// Buffers for encryption
	unsigned char in_buffer[BUFFER_SIZE];
	unsigned char encrypted_buffer[BUFFER_SIZE];
	unsigned char out_buffer[BUFFER_SIZE];


	// Copy seed string to char array;
	unsigned char key[seed.size()];


	// Convert the seed to a char array and pass it to a oointer
	memcpy( (char*) key, seed.c_str(), seed.size());


	// Start encryption
	AES_KEY enc_key; 
	AES_set_encrypt_key(key, 128, &enc_key);


	// Copy password to buffer
	memcpy( (char*) in_buffer, password.c_str(), password.size());
	printf("inbuffer: %s\n",in_buffer);


	// Encypt stream from inbuffer to encrypted buffer
	AES_encrypt(in_buffer, encrypted_buffer, &enc_key);
	printf("encryptedbuffer: %s\n", encrypted_buffer);


	// Copy encrypted_buffer to output variable
	for (long unsigned int i=0; i<sizeof(encrypted_buffer); i++){
		output[i] = encrypted_buffer[i];
	}

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



// Encrypt the password and write it to a file
bool encrypt_and_write_password(string password, string seed, string hint){
	
	// Encrypt the password into an unsigned char array
	unsigned char encrypted[BUFFER_SIZE];
	encrypt_password(password, seed, encrypted);

	// Will abstract out the following steps....

	// Write unsigned char array to file
	string file_path = "./.store/passwords/"+hint;
	const char* file = file_path.c_str();

	ofstream f(file);
	f.write((char *)&encrypted[0], BUFFER_SIZE);
	f.close();

	return 1;
}

bool decrypt_and_read_password(string seed, string hint){

	// Declare new char array
	unsigned char new_encrypted[BUFFER_SIZE];
	
	// Read unsigned char array from file
	string file_path = "./.store/passwords/"+hint;
	const char* file = file_path.c_str();

	ifstream g(file);
	g.read((char *)&new_encrypted[0], BUFFER_SIZE);
	g.close();

	// Decrypt
	decrypt_password(new_encrypted, seed);

	return 1;
}

