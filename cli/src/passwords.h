// Functions for managing passwords locally
#ifndef _PASSWORDS_H
#define _PASSWORDSS_H
#include <string>
using namespace std;

bool write_seed(string seed);

bool destroy_seed();

string get_seed();


#endif