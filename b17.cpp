#include "readIn.h"
#include "process.h"

instMap instructions = {
	{"0000", {{"00", "HALT"}, {"01", "LD"}, {"10", "ADD"}, {"11", "J"}}},
	{"0001", {{"00", "NOP"}, {"01", "ST"}, {"10", "SUB"}, {"11", "JZ"}}},
	{"0010", {{"00", "?"}, {"01", "EM"}, {"10", "CLR"}, {"11", "JN"}}},
	{"0011", {{"00", "?"}, {"01", "?"}, {"10", "COM"}, {"11", "JP"}}},
	{"0100", {{"00", "?"}, {"01", "?"}, {"10", "AND"}, {"11", "?"}}},
	{"0101", {{"00", "?"}, {"01", "?"}, {"10", "OR"}, {"11", "?"}}},
	{"0110", {{"00", "?"}, {"01", "?"}, {"10", "XOR"}, {"11", "?"}}},
	{"0111", {{"00", "?"}, {"01", "?"}, {"10", "?"}, {"11", "?"}}},
	{"1000", {{"00", "?"}, {"01", "LDX"}, {"10", "ADDX"}, {"11", "?"}}},
	{"1001", {{"00", "?"}, {"01", "STX"}, {"10", "SUBX"}, {"11", "?"}}},
	{"1010", {{"00", "?"}, {"01", "EMX"}, {"10", "CLRX"}, {"11", "?"}}},
	{"1011", {{"00", "?"}, {"01", "?"}, {"10", "?"}, {"11", "?"}}},
	{"1100", {{"00", "?"}, {"01", "?"}, {"10", "?"}, {"11", "?"}}},
	{"1101", {{"00", "?"}, {"01", "?"}, {"10", "?"}, {"11", "?"}}},
	{"1110", {{"00", "?"}, {"01", "?"}, {"10", "?"}, {"11", "?"}}},
	{"1111", {{"00", "?"}, {"01", "?"}, {"10", "?"}, {"11", "?"}}}
};

unordered_map <string, int> aModes= {
	{"0000", 0},	//Direct
	{"0001", 1},	//Immediate
	{"0010", 2},	//Indexed
	{"0100", 3},	//Indirect
	{"0110", 4}		//Indirect Indexed
};

unordered_map <string, vector<bool>> instAMs = {
	{"HALT", {true, true, true, true}}, 	// Ignored.
	{"NOP", {true, true, true, true}}, 		// Ignored
	{"LD", {true, true, true, true}}, 		// All.
	{"ST", {true, false, true, true}}, 		// All except Immediate.
	{"EM", {true, false, true, true}}, 		// All except Immediate.
	{"LDX", {true, true, false, false}},	// Direct, Immediate.
	{"STX", {true, false, false, false}}, 	// Direct.
	{"EMX", {true, false, false, false}}, 	// Direct.
	{"ADD", {true, true, true, true}}, 		// All.
	{"SUB", {true, true, true, true}}, 		// All.
	{"CLR", {true, true, true, true}}, 		// Ignored.
	{"COM", {true, true, true, true}}, 		// Ignored.
	{"AND", {true, true, true, true}}, 		// All.
	{"OR", {true, true, true, true}}, 		// All.
	{"XOR", {true, true, true, true}}, 		// All.
	{"ADDX", {true, true, false, false}}, 	// Direct, Immediate.
	{"SUBX", {true, true, false, false}}, 	// Direct, Immediate.
	{"CLRX", {true, true, true, true}}, 	// Ignored.
	{"J", {true, false, true, true}}, 		// All except Immediate.
	{"JZ", {true, false, true, true}}, 		// All except Immediate.
	{"JN", {true, false, true, true}},		// All except Immediate.
	{"JP", {true, false, true, true}} 		// All except Immediate.
};

string MAR; 	// Memory Address Register Address of the memory location which is to be loaded from or stored into.
string IC; 		// Instruction Counter Address of the next instruction to be fetched, decoded, and executed.
string X0; 		// Index Registers Four registers (X0-X3); contain values to be used in calculating memory addresses.
string X1; 		// Index Registers Four registers (X0-X3); contain values to be used in calculating memory addresses.
string X2; 		// Index Registers Four registers (X0-X3); contain values to be used in calculating memory addresses.
string X3; 		// Index Registers Four registers (X0-X3); contain values to be used in calculating memory addresses.
string ABUS; 	// Address Bus Used when addresses are to be moved.
string MDR; 	// Memory Data Register Data to be written into, or data most recently read from, memory.
string AC = "0"; 		// Accumulator The accumulator register.
string ALU; 	// Arithmetic-Logic Unit Performs computations.
string IR; 		// Instruction Register Instruction being decoded and executed.
string DBUS; 	// Data Bus Used when data and instructions are to be moved.
string PC;		// Program counter
hexMap hexProg;
binMap binProg;


int main( int argc, char** argv )
{

	// usage error
	if (argc != 2)
	{
		cout << "comandline argument error" << endl;
		cout << "Usage: ./b17 <filename.obj>" << endl;
		cout << "Example: ./b17 prog.obj" << endl;
		return 1;
	}	

	// data container prep
	ifstream inFile (argv[1]);
	
	// file open error
	if ( !inFile.is_open() )
	{
		cout << "Unable to open file" << endl;
		return 1;
	}

	// read in data
	PC = readIn (inFile);

	fetchInstruction();
	return 0;
}


