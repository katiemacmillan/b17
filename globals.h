#ifndef _globals_H_
#define _globals_H_

#include <fstream>
#include <iomanip>
#include <istream>
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <cstdlib>	//need c stdlib for exit()

using namespace std;

typedef unordered_map<string, vector<int>> hexMap;
typedef unordered_map<string, vector<string>> binMap;
typedef unordered_map <string, unordered_map<string, string>> instMap;


#endif