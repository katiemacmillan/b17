#ifndef _readIn_H_
#define _readIn_H_
#include "globals.h"

extern binMap binProg;

string readIn (ifstream&);
void parseValues (const string&, vector<string>&);

#endif