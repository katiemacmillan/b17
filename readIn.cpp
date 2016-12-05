#include "readIn.h"



string readIn (ifstream& inFile)
{
	string address;
	string trash;
	string values;
	vector<string> binList;	
	while (!inFile.eof())
	{
		// read in movie title & cast
		getline(inFile, address, ' ');
        bitset<24> b(stoi(address, 0, 16));

		if ( getline(inFile, trash, ' ')){
			getline(inFile, values);
			// parse cast into list
			parseValues(values, binList);
			// add title and cast to movie hashtable
			binProg.emplace(b.to_string(), binList);
			binList.clear();
		}
	}
	bitset<24> b(stoi(address, 0, 16));
	return b.to_string();
}


void parseValues (const string& str, vector<string>& bin)
{
    // skip delimiters to start of first token
    int tokenStart = str.find_first_not_of( ' ', 0 );
    // find next delimiter (i.e., end of first token)
    int tokenEnd = str.find_first_of( ' ', tokenStart );

    // loop through input string
    while ( tokenStart != (signed int)string::npos )
    {
    	string sub =  str.substr( tokenStart, tokenEnd - tokenStart );
    	int v = stoi(sub, 0, 16);
        bitset<24> b(v);
        // found a token, add it to the vector
        bin.push_back(b.to_string());
        // skip delimiters to start of next token
        tokenStart = str.find_first_not_of( ' ', tokenEnd );
        // find next delimiter (end of token)
        tokenEnd = str.find_first_of( ' ', tokenStart );
    }
}
