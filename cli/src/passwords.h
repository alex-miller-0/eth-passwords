// Functions for managing passwords locally
#ifndef _PASSWORDS_H
#define _PASSWORDSS_H
#include <string>
using namespace std;

// Helper functions
string pad_string(string input, int size);
void encrypt_password(string password, string seed, unsigned char output[]);
string decrypt_password(unsigned char encrypted_password[], string seed);

// Seed functions
bool write_seed(string seed);
bool destroy_seed();
string get_seed();

// Password file related functions
bool write_password(string password, string seed, string hint);

#endif