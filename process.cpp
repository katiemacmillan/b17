
#include "process.h"

void executeProgram(){
	bool keepGoing = true;
	while (keepGoing){
		fetchInstruction();
		fetchAddress();
		fetchOp();
		fetchData();
		executeInstruction();
		keepGoing = printTrace();
		if (!keepGoing){
			return;
		}
		moveProgCounter();
	}

}
void fetchAddress(){
	ABUS = IR.substr(0, 12);	// Copy address bits of instruction to Adr Bus
	MAR = ABUS;					// Move address to Mem Adr Reg
	// It's a string, padd the begining with 0's
	if(24 > MAR.size()) {
		MAR.insert(0, 24 - MAR.size(), '0');
	}
	Reg = stoi(IR.substr(IR.length()-2, 2), 0, 2);
}
void fetchInstruction(){
	ABUS = IC;		// Load address of Inst Ct into Adr Bus
	MAR = ABUS;		// Load address from bus into Mem Adr Reg
	loadMem();		// Load data from MAR into Mem Data Reg
	IR = MDR;		// Set Inst Reg to data in Mem Data Reg
}

void fetchOp(){
	string op = IR.substr(IR.length()-12, 6);
	instMap::const_iterator check = instructions.find (op);
	if ( check != instructions.end() ){
		OpR = instructions.at(op);
	} else {
		OpR = "????";
	}

}
void fetchData(){
	string aM = IR.substr(IR.length()-6, 4);

	modeMap::const_iterator check = adrModes.find (aM);
	if ( check != adrModes.end() ){
		AM = adrModes.at(aM);
	} else {
		AM = 5;
	}

	if ((AM != 4) && (OpR != "????") && (!instAMs.at(OpR).at(AM))){
		AM = 5;
	}
	if ((OpR == "HALT") || (OpR == "NOP")){
		AM = 6;
	}
	
	// Deal with memory addressing
	switch (AM){
		case 0:	//direct
			loadMem();
		break;
		case 1: //Immediate
			MDR = MAR;
		break;
		case 2: //Indexed
			// Add index to MAR address
			loadMem();
		break;
		case 3: // Indirect
			loadMem();			// Go fetch address from address in MAR
			MAR = MDR;			// Set MAR to new address in MDR
			loadMem();			// Load data from new address
		break;
	}
}

void loadMem (){
	bool found = false;
	int i = 0;
	long b = stol(MAR, 0, 2);
	string adr = MAR;
	do{
		binMap::const_iterator check = binProg.find (adr);
  		if ( check == binProg.end() ){
  			b--;
			i++;
			adr = bitset< 24 >( b ).to_string();
			found = false;
  		} else {
			found = true;
  		}
	}while (!found);
	DBUS = binProg.at(adr).at(i);		// Load data from mem location into data bus 
	MDR = DBUS;							// Load data in bus to Mem Data Reg
}

void storeMem (){
	bool found = false;
	long b = stol(MAR, 0, 2);
	int i = 0;
	string adr = MAR;
	DBUS = MDR;							// Move data from Mem Data Reg into Data Bus
	do{
		binMap::const_iterator check = binProg.find (adr);
  		if ( check == binProg.end() ){
  			b--;
			i++;
			adr = bitset< 24 >( b ).to_string();
			found = false;
  		} else {
			found = true;
  		}
	}while (!found);
	
	binProg.at(adr).at(i) = DBUS;		// Mode data from bus into mem location
}

void executeInstruction(){
	long ac = stol(AC, 0, 2);
	long x = stol(X[Reg], 0, 2);
	//execute instruction

	if (OpR == "LD")
	{
		AC = bitset< 24 >( MDR ).to_string();		// Move loaded data from Mem Data Reg to AC
	}
	else if (OpR == "LDX")
	{
		X[Reg] = MDR;		// Move loaded data from Mem Data Reg to reg X[n]
		if (X[Reg].size() > 12)
			X[Reg] = X[Reg].substr(0,12);

	}
	else if (OpR == "ST")
	{
		//store
		storeMem();
	}
		else if (OpR == "STX")
	{
		MDR = X[Reg];
		//store
		storeMem();
	}
	else if (OpR == "EM")
	{
		//exchange AC with memory
		DBUS = AC;
		loadMem();
		AC = MDR;
		MDR = DBUS;
		storeMem();
	}
	else if (OpR == "EMX")
	{
		//exchange AC with memory
		DBUS = X[Reg];
		loadMem();
		X[Reg] = MDR;
		if (X[Reg].size() > 12){
			X[Reg] = X[Reg].substr(X[Reg].size()-12, 12);
		}
		MDR = DBUS;
		storeMem();
	}
	else if (OpR == "ADD")
	{
		ArithLogUnit(ac, 1);
		AC = bitset< 24 >( ALU ).to_string();
	}
	else if (OpR == "ADDX")
	{
		//add memory to AC
		ArithLogUnit(x, 1);
		X[Reg] = bitset< 12 >( ALU ).to_string();
	}
	else if (OpR == "SUB")
	{
		ArithLogUnit(ac, 2);
		AC = bitset< 24 >( ALU ).to_string();
	}
	else if (OpR == "SUBX")
	{
		ArithLogUnit(x, 2);
		X[Reg] = bitset< 12 >( ALU ).to_string();
	}
	else if (OpR == "CLR")
	{
		//clear AC
		ArithLogUnit(ac, 3);
		AC = bitset< 24 >( ALU ).to_string();
	}
	else if (OpR == "CLRX")
	{
		//clear AC
		ArithLogUnit(x, 3);
		X[Reg] = bitset< 12 >( ALU ).to_string();
	}
	else if (OpR == "COM")
	{
		//compliment AC
		ArithLogUnit(ac, 4);
		AC = bitset< 24 >( ALU ).to_string();

	}
	else if (OpR == "AND")
	{
		//AND AC with memory
		ArithLogUnit(ac, 5);
		AC = bitset< 24 >( ALU ).to_string();
	}
	else if (OpR == "OR")
	{
		//OR AC with memory
		ArithLogUnit(ac, 6);
		AC = bitset< 24 >( ALU ).to_string();
	}
	else if (OpR == "XOR")
	{
		ArithLogUnit(ac, 7);
		//XOR AC with memory
		AC = bitset< 24 >( ALU ).to_string();
	}
}
void ArithLogUnit(long data, int op){
	long mdr = stol(MDR, 0, 2);

	switch(op){
		case 1:				//ADD
			ALU = data + mdr;
		break;
		case 2:				//Subtract
			ALU = data - mdr;
		break;
		case 3:				//Clear
			ALU = data ^ data;
		break;
		case 4:				//Compliment
			ALU = ~data;
		break;
		case 5:				//AND
			ALU = data & mdr;
		break;
		case 6:				//OR
			ALU = data | mdr;
		break;
		case 7:				//XOR
			ALU = data ^ mdr;
		break;
	}

}
bool printTrace() {
	long ic = stol(IC, 0, 2);
	// string ac = to_string(stol(AC, 0, 2));
	stringstream stream;
	stream << hex << stol(AC, 0, 2);
	string ac = stream.str();
	if (ac.size() > 6){
		ac = ac.substr(ac.size()-6, 6);
	}
	//output instruction information
	cout << hex << setfill('0') << setw(3) << right << ic << ":  "
		<< setw(6) << stol(IR,0,2) << "  " << setw(4) << left << setfill(' ')
		<< OpR << setfill('0') << right << "  ";

	if ((OpR == "HALT") || (OpR == "NOP") || (OpR == "CLR") || (OpR == "COM"))
		//instruction ignores addressing mode
		cout << "     ";
	else {
		if (AM == 1){
			cout << setw(3) << "IMM  ";

		} else if ((AM == 4) || (AM == 5)){
			cout << setw(3) << "???  ";
		}else {
			cout << setw(3) << hex << stol(MAR, 0, 2) << "  ";			
		}
	}
	
	// cout << "AC: " << AC << endl;
	//more instruction info
	cout << "AC[" << right << setw(6) << hex << ac << "]  ";
	for (int i = 0; i < 4; i++){
		stringstream strm;
		strm << hex << stoi(X[i], 0, 2);
		string x = strm.str();
		// if (x.size() > 3){
		// 	x = x.substr(x.size()-3, 3);
		// }	
		cout << "X" << i << "[" << setfill('0') << setw(3)<< setprecision(3) <<  x ;
		if (i < 3) {
			cout << "]  ";
		} else {
			cout << "]" << endl;
		}
	}

	//HALT the machine
	if (OpR == "HALT") {
		cout << "Machine Halted - HALT instruction executed" << endl;
		return false;
	} else if ((OpR == "????")) {
		cout << "Machine Halted - undefined instruction" << endl;
		return false;
	} else if (AM == 4){
		cout << "Machine Halted - unimplemented addressing mode" << endl;
		return false;
	} else if (AM == 5){
		cout << "Machine Halted - illegal addressing mode" << endl;
		return false;
	} else {
		return true;
	}
}

void moveProgCounter (){
	long ac = stol(AC, 0, 2);
	long ic = stol(IC, 0, 2);
	//jump instructions
	if (OpR == "J")
		IC = MAR;
	else if ((OpR == "JN") && (ac < 0))
		IC = MAR;
	else if ((OpR == "JP") && (ac > 0))
		IC = MAR;
	else if ((OpR == "JZ") && (ac == 0))
		IC = MAR;
	else
		IC = bitset< 12 >( ic + 1 ).to_string();

}
