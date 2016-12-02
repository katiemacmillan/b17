
#include "process.h"
// Go to address
// fetch data

// get addressing mode
// get address
// get op
// get instruction
void fetchInstruction(){
	bool keepGoing = true;
	string data;
	string adr;
	string op11;
	string op9;
	string aM ;
	string instruction;
	int aMode;
	while (keepGoing){
		data = loadMem(PC);

		adr = data.substr(data.length()-24, 12);
		op11 = data.substr(data.length()-12, 2);
		op9 = data.substr(data.length()-10, 4);
		aM = data.substr(data.length()-6, 4);

		if(32 > adr.size())
        adr.insert(0, 32 - adr.size(), '0');
		try {
			instruction = instructions.at(op9).at(op11);
		} catch (int e){
			instruction = "?";
		}

		try {
			aMode = aModes.at(aM);
		} catch (int e){
			// Not valid addressing Mode
			aMode = 5;
		}

		if ((aMode != 4) && (!instAMs.at(instruction).at(aMode))){
			aMode = 5;
		}
		if ((instruction == "HALT")){
			aMode = 6;
		}

		int value;
		// Deal with memory addressing
		switch (aMode){
			case 0:	//direct
				//Get adr and index into adr
				value = stoi(loadMem(adr), 0, 2);
			break;
			case 1: //Immediate
				value = stoi(adr, 0, 2);
			break;
			case 2: //Indexed
			break;
			case 3: // Indirect
			break;
		}

		executeInstruction(instruction, adr, value);
		keepGoing = printTrace(instruction, adr, aMode);
		if (!keepGoing){
			return;
		}
		moveProgCounter(instruction, adr);
		//get instruction from memory
		// data = loadMem(PC);
	}

}

void moveProgCounter (string instruction, string adr){
	int ac = stoi(AC, 0, 2);
	int pc = stoi(PC, 0, 2);
	//jump instructions
	if (instruction == "J")
		PC = adr;
	else if ((instruction == "JN") && (ac < 0))
		PC = adr;
	else if ((instruction == "JP") && (ac > 0))
		PC = adr;
	else if ((instruction == "JZ") && (ac == 0))
		PC = adr;
	else
		PC = bitset< 32 >( pc + 1 ).to_string();
}

bool printTrace(string instruction, string adr, int aMode) {
	int pc = stoi(PC, 0, 2);
	//output instruction information
	cout << hex << setfill('0') << setw(3) << right << pc << ":  "
		<< setw(6) << instruction << "  " << setw(4) << left << setfill(' ')
		<< instruction << setfill('0') << right << "  ";

	if ((instruction == "HALT") || (instruction == "NOP") || (instruction == "CLR") || (instruction == "COM"))
		//instruction ignores addressing mode
		cout << "     ";
	else {
		switch (aMode){
			case 0:
				cout << setw(3) << hex << stoi(adr, 0, 2) << "  ";
			break;
			case 1:
				cout << setw(3) << "IMM  ";
			break;
			case 2:
				cout << setw(3) << "IMM  ";
			break;
			case 3:
				cout << setw(3) << "IMM  ";
			break;
			default:
				cout << "???  ";
			break;
		}
	}
	
	//more instruction info
	cout << "AC[" << right << setw(6) << hex << stoi(AC, 0, 2) << "]  ";
	cout << "X0[000]  X1[000]  X2[000]  X3[000]" << endl;

	//HALT the machine
	if (instruction == "HALT") {
		cout << "Machine Halted - HALT instruction executed" << endl;
		return false;
	}
	//unimplemented code
	if ((instruction == "LDX") || (instruction == "STX") || (instruction == "EMX")
		|| (instruction == "ADDX") || (instruction == "SUBX") || (instruction == "CLRX")) {
		cout << "Machine Halted - unimplemented instruction" << endl;
		return false;
	}

	//invalid instruction
	if ((instruction == "?")) {
		cout << "Machine Halted - undefined instruction" << endl;
		return false;
	}

	switch (aMode){
		case 4:
			cout << "Machine Halted - unimplemented addressing mode" << endl;
			return false;
		break;
		case 5:
			cout << "Machine Halted - illegal addressing mode" << endl;
			return false;
		break;
	}
	return true;
}

string loadMem (string adr){
	bool found = false;
	int i = 0;
	int b = stoi(adr, 0, 2);
	do{
		binMap::const_iterator check = binProg.find (adr);
  		if ( check == binProg.end() ){
  			b--;
			i++;
			adr = bitset< 32 >( b ).to_string();
			found = false;
  		} else {
			found = true;
  		}
	}while (!found);
	return binProg.at(adr).at(i);
}

void storeMem (string adr, string data){
	bool found = false;
	int i = 0;
	int b = stoi(adr, 0, 2);
	do{
		binMap::const_iterator check = binProg.find (adr);
  		if ( check == binProg.end() ){
  			b--;
			i++;
			adr = bitset< 32 >( b ).to_string();
			found = false;
  		} else {
			found = true;
  		}
	}while (!found);
	
	binProg.at(bitset< 32 >( b ).to_string()).at(i) = data;
}

void executeInstruction( string instruction, string adr, int value){
	int ac = stoi(AC, 0, 2);

	//execute instruction
	if (instruction == "LD")
	{
		//load
		AC = bitset< 32 >( value ).to_string();
	}
	else if (instruction == "ST")
	{
		//store
		storeMem(adr, AC);
	}
	else if (instruction == "EM")
	{
		//exchange AC with memory
		string temp = AC;
		AC = loadMem(adr);
		storeMem(adr, temp);
	}
	else if (instruction == "ADD")
	{
		//add memory to AC
		if (ac < 0)
			ac--;
		ac += value;
		AC = bitset< 32 >( ac ).to_string();
	}
	else if (instruction == "SUB")
	{
		//substract memory from AC
		ac -= value;
		AC = bitset< 32 >( ac ).to_string();
	}
	else if (instruction == "CLR")
	{
		//clear AC
		ac = ac ^ ac;
		AC = bitset< 32 >( ac ).to_string();
	}
	else if (instruction == "COM")
	{
		//compliment AC
		ac = ~ac;
		AC = bitset< 32 >( ac ).to_string();

	}
	else if (instruction == "AND")
	{
		//AND AC with memory
		ac = ac & value;
		AC = bitset< 32 >( ac ).to_string();

	}
	else if (instruction == "OR")
	{
		//OR AC with memory
		ac = ac | value;
		AC = bitset< 32 >( ac ).to_string();
	}
	else if (instruction == "XOR")
	{
		//XOR AC with memory
		ac = ac ^ value;
		AC = bitset< 32 >( ac ).to_string();
	}
}