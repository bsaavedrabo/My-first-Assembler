// Assembler Project 6, Computer Organization:
// Belen Saavedra

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>


using namespace std;

string userDefinedFile(){
	// ask the user to define the file they need to translate 
	string userDefFile;
	cout << "Indicate the file that you want to translate. Make sure it ends with .asm: " << endl; 
	cin >> userDefFile; 
	cout << "" << endl;
	cout << "Processing " << userDefFile << "...."<< endl;
	return userDefFile;
}

class Assembler {

public:

	Assembler() {

		JumpBit = { {" ", "000"}, {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"}, {"JLT", "100"}, {"JNE", "101"}, {"JLE", "110"},{"JMP", "111"} };

		DestBit= { {" ", "000"}, {"M", "001"},{"D","010"}, {"MD=", "011"}, {"A=", "100"},{"AM=", "101"}};

		CompBit = {{"1", "0111111"},{"-1", "0111010"},{"D", "0001100"},{"A", "0110000"},{"!D", "0001101"},{"!A", "0110001"},{"-D", "0001111"},{"-A", "0110011"},{"1+D", "0011111"},
			{"1+A", "0110111"},{"D-1", "0001110"},{"A-1", "0110010"},{"D+A", "0000010"},{"D-A", "0010011"},{"A-D", "0000111"},{"D&A", "0000000"},{"D|A", "0010101"},
			{"M", "1110000"},{"!M", "1110001"},{"-M", "1110011"},{"M+1", "1110111"},{"M-1", "1110010"},{"D+M", "1000010"},{"D-M", "1010011"},
			{"M-D", "1000111"},{"D&M", "1000000"},{"D|M", "1010101"}};

		Symbols = { {"SP", 0},{"LCL", 1}, {"ARG", 2},{"THIS", 3},{"THAT", 4},{"SCREEN", 16384},{"KBD", 24576},{"R0", 0},{"R1", 1},{"R2", 2},{"R3", 3},{"R4", 4},
	               {"R5", 5},{"R6", 6},{"R7", 7},{"R8", 8},{"R9", 9},{"R10", 10},{"R11", 11},{"R12", 12},{"R13", 13},{"R14", 14},{"R15", 15}};
	}

	void SaveonFile(string line, fstream& targetfile) {
		// save new line into the target file. 

	
		targetfile.seekp(0, ios::end);
		if (targetfile.tellp() > 0) {
			targetfile.seekp(-1, ios::out);
			char last = targetfile.get();

			if (last != '\n') {
				targetfile << "\n";
			}
		
		
		}
	targetfile << line << "\n";
	
	}

	string TranslateBinary(int decimalVal) {
		// Translate the decimal values into binary 
		string binary;
		
		if (decimalVal == 0) { return "0000000000000000"; }

		else {
			while (decimalVal > 0) {
				binary = to_string(decimalVal % 2) + binary;
				decimalVal /= 2;
			}

			while (binary.length() < 16) { // add 0s as neeeded. 
				binary = "0" + binary;
			}
		}
		return binary;
	}

		void Run_(string line, fstream & tfile) {
			// Remove any whitespace at the beginning and end of the line
			line.erase(0, line.find_first_not_of(" " || "//"));
			line.erase(line.find_last_not_of(" ") + 1);


			// If it is an A instruction
			if (line[0] == '@') {

				// 1) if it is storing an integer

				if (line[1] != 'R') {
					string sub_string = line.substr(1);
					string binary_code = TranslateBinary(stoi(sub_string));
					cout << binary_code << endl;
				}
				//SaveonFile(binary_code, tfile);

				// 2) if it is storing the address of a register
				else if (line[1] == 'R') {
					string sub = line.substr(2); 
					string binary = TranslateBinary(stoi(sub));
					cout << binary << endl;
				}

				// 3) if the A instruction is a variable 
				/*else {
					string var_name = line.substr(1); // remove the '@' symbol
					int var_address;
					if (Symbols.find(var_name) == Symbols.end()) {
						// Variable not found in symbol table, so assign a new memory address
						var_address = next_variable_address;
						Symbols[var_name] = next_variable_address;
						next_variable_address++;
					}
					else {
						// Variable already exists in symbol table, so use its assigned memory address
						var_address = Symbols[var_name];
					}
					string binary_code = TranslateBinary(var_address);
					cout << binary_code << endl;
				}*/

			}


			// If it is a C instruction
			else if (line.find('=') != string::npos || line.find(';') != string::npos) {
				string dest_bits = "000";
				string comp_bits;
				string jump_bits = "000";
				size_t eq_pos = line.find('=');
				size_t sc_pos = line.find(';');

				// Check if the instruction has a destination
				if (eq_pos != string::npos) {
					dest_bits = DestBit[line.substr(0, eq_pos)];
					dest_bits.erase(remove(dest_bits.begin(), dest_bits.end(), ' '), dest_bits.end());
					comp_bits = CompBit[line.substr(eq_pos + 1)];
				}

				// If the instruction has a jump
				if (sc_pos != string::npos) {
					jump_bits = JumpBit[line.substr(sc_pos + 1)];
				}
				// Extract the comp bits substring from the right position
				if (sc_pos != string::npos) {
					comp_bits = CompBit[line.substr(0, sc_pos)];
				}
				else if (eq_pos != string::npos) {
					comp_bits = CompBit[line.substr(eq_pos + 1)];
				}
				else {
					comp_bits = CompBit[line];
				}
				string binary_code = "111" + comp_bits + dest_bits + jump_bits;
				cout << binary_code << endl; 
				//SaveonFile(binary_code, tfile);
			}
		}
		
	

private: 
	 
	unordered_map<string, string> JumpBit;
	unordered_map<string, string> CompBit;
	unordered_map<string, string> DestBit;
	unordered_map<string, int> Symbols;
	int next_variable_address = 16;
};


int main() {
	string readstring;

	// Get file from the user
	string attach = "testfiles/";
	//attach += userDefinedFile();
	
	fstream file("testfiles/Add.asm");
	fstream targetfile("targetfile.hack");
	//targetfile.open("targetfile.hack", ios::out);
	
	Assembler test;
	//cout << TranslateBinary(2);

	if (file.is_open()&& targetfile.is_open()) { 
		while (getline(file, readstring)) {
			
			test.Run_(readstring, targetfile);// this is the function that runs the Assembler class. 
			

		}
		file.close();
	}
	else {
		cout << "Unable to open file. It is possible that an unexisting file was specified or .asm was not added, try again ..." << endl; cout << " Available files: Add.asm, Max.asm, Pong.asm, Rect.asm" << endl;
	}
	cin >> readstring; // This is to keep screen open

	return 0;
}
