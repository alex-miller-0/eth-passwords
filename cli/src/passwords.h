// Functions for managing passwords locally
#ifndef _PASSWORDS_H
#define _PASSWORDSS_H
#include <string>
using namespace std;

// Helper functions
string pad_string(string input, int size);
string check_seed(string seed);
void convert_string(string str, int size, unsigned char to_convert[]);

// Encryption
void encrypt_password(string password, string seed, unsigned char output[]);
string decrypt_password(unsigned char encrypted_password[], string seed);

// Seed functions
bool write_seed(string seed);
bool destroy_seed();
string get_seed();


// Password file related functions
bool encrypt_and_write_password(string password, string seed, string hint);
bool decrypt_and_read_password(string seed, string hint);

#endif