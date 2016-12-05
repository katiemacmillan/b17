#include "readIn.h"
#include "process.h"

instMap instructions = {
	{"000000", "HALT"},
	{"000001", "NOP"},
	{"110000", "J"},
	{"110001", "JZ"},
	{"110010", "JN"},
	{"110011", "JP"},
	{"010000", "LD"},
	{"011000", "LDX"},
	{"010001", "ST"},
	{"011001", "STX"},
	{"010010", "EM"},
	{"011010", "EMX"},
	{"100000", "ADD"},
	{"101000", "ADDX"},
	{"100001", "SUB"},
	{"101001", "SUBX"},
	{"100010", "CLR"}, 
	{"101010", "CLRX"},
	{"100011", "COM"}, 
	{"100100", "AND"},
	{"100101", "OR"},
	{"100110", "XOR"},
};

modeMap adrModes= {
	{"0000", 0},	//Direct
	{"0001", 1},	//Immediate
	{"0010", 2},	//Indexed
	{"0100", 3},	//Indirect
	{"0110", 4}		//Indirect Indexed
};

instModeMap instAMs = {
	{"HALT", {true, true, true, true}}, 	// Ignored.
	{"NOP", {true, true, true, true}}, 		// Ignored
	{"J", {true, false, true, true}}, 		// All except Immediate.
	{"JZ", {true, false, true, true}}, 		// All except Immediate.
	{"JN", {true, false, true, true}},		// All except Immediate.
	{"JP", {true, false, true, true}}, 		// All except Immediate.
	{"LD", {true, true, true, true}}, 		// All.
	{"LDX", {true, true, false, false}},	// Direct, Immediate.
	{"ST", {true, false, true, true}}, 		// All except Immediate.
	{"STX", {true, false, false, false}}, 	// Direct.
	{"EM", {true, false, true, true}}, 		// All except Immediate.
	{"EMX", {true, false, false, false}}, 	// Direct.
	{"ADD", {true, true, true, true}}, 		// All.
	{"ADDX", {true, true, false, false}}, 	// Direct, Immediate.
	{"SUB", {true, true, true, true}}, 		// All.
	{"SUBX", {true, true, false, false}}, 	// Direct, Immediate.
	{"CLR", {true, true, true, true}}, 		// Ignored.
	{"CLRX", {true, true, true, true}}, 	// Ignored.
	{"COM", {true, true, true, true}}, 		// Ignored.
	{"AND", {true, true, true, true}}, 		// All.
	{"OR", {true, true, true, true}}, 		// All.
	{"XOR", {true, true, true, true}} 		// All.
};

string IC; 		// Instruction Counter Address of the next instruction to be fetched, decoded, and executed.
string IR; 		// Instruction Register Instruction being decoded and executed.
string X[4] = {"0","0","0","0"}; 		// Index Registers Four registers (X0-X3); contain values to be used in calculating memory addresses.
string MAR; 	// Memory Address Register Address of the memory location which is to be loaded from or stored into.
string MDR; 	// Memory Data Register Data to be written into, or data most recently read from, memory.
string AC = "0"; 		// Accumulator The accumulator register.
long ALU; 	// Arithmetic-Logic Unit Performs computations.
string ABUS; 	// Address Bus Used when addresses are to be moved.
string DBUS; 	// Data Bus Used when data and instructions are to be moved.
string OpR;		// Op Instruction to be executed
int Reg;		// Register Used
bool Neg;		// Negate a value
int AM;			// Addressing Mode
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
	IC = readIn (inFile);
	executeProgram();
	return 0;
}


