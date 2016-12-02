#ifndef _process_H_
#define _process_H_
// #include <string>
#include "globals.h"
// using namespace std;

extern hexMap hexProg;
extern binMap binProg;

extern instMap instructions;
extern string MAR; 	// Memory Address Register Address of the memory location which is to be loaded from or stored into.
extern string IC; 		// Instruction Counter Address of the next instruction to be fetched, decoded, and executed.
extern string X0; 		// Index Registers Four registers (X0-X3); contain values to be used in calculating memory addresses.
extern string X1; 		// Index Registers Four registers (X0-X3); contain values to be used in calculating memory addresses.
extern string X2; 		// Index Registers Four registers (X0-X3); contain values to be used in calculating memory addresses.
extern string X3; 		// Index Registers Four registers (X0-X3); contain values to be used in calculating memory addresses.
extern string ABUS; 	// Address Bus Used when addresses are to be moved.
extern string MDR; 	// Memory Data Register Data to be written into, or data most recently read from, memory.
extern string AC; 		// Accumulator The accumulator register.
extern string ALU; 	// Arithmetic-Logic Unit Performs computations.
extern string IR; 		// Instruction Register Instruction being decoded and executed.
extern string DBUS; 	// Data Bus Used when data and instructions are to be moved.
extern string PC;		// Program counter
extern unordered_map <string, int> aModes;
extern unordered_map <string, vector<bool>> instAMs;
void fetchInstruction();
string loadMem (string);
void storeMem (string, string);
bool printTrace(string, string, int);
void moveProgCounter (string, string);
void executeInstruction(string, string, int);
#endif