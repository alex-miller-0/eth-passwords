// Functions for creating and managing passwords locally
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

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