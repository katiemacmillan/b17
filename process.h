#ifndef _process_H_
#define _process_H_
// #include <string>
#include "globals.h"
// using namespace std;

extern binMap binProg;

extern instMap instructions;
extern string MAR; 	// Memory Address Register Address of the memory location which is to be loaded from or stored into.
extern string IC; 		// Instruction Counter Address of the next instruction to be fetched, decoded, and executed.
extern string X[4]; 		// Index Registers Four registers (X0-X3); contain values to be used in calculating memory addresses.
extern string ABUS; 	// Address Bus Used when addresses are to be moved.
extern string MDR; 		// Memory Data Register Data to be written into, or data most recently read from, memory.
extern string AC; 		// Accumulator The accumulator register.
extern long ALU; 		// Arithmetic-Logic Unit Performs computations.
extern string IR; 		// Instruction Register Instruction being decoded and executed.
extern string DBUS; 	// Data Bus Used when data and instructions are to be moved.
extern string OpR;		// Op Instruction to be executed
extern int Reg;		// Register Used
extern int AM; 		// Addressing Mode Being Used
extern modeMap adrModes;
extern instModeMap instAMs;
void executeProgram();
void fetchAddress();
void fetchInstruction();
void loadMem ();
void storeMem ();
bool printTrace();
void moveProgCounter ();
void ArithLogUnit(long, int);
void executeInstruction();
void fetchOp();
void fetchData();
#endif