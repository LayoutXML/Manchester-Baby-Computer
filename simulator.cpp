/*
	Names: Calum Logan, Emilija Budryte, Rokas Jankunas, Jokubas Butkus, & Momchil Badzhev
	simulator.cpp written by: Rokas Jankunas, Jokubas Butkus, & Momchil Badzhev
	Matriculation Numbers: 180013466, 180017115, 180003228, 180017346, 180012872
	Module Code: AC21009
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <cmath>
#include <unistd.h>

using namespace std;

void decode();
void fetch();
void execute();
int binaryToDecimal(vector<bool> binaryVector);
int binaryToDecimalArray(array<bool, 32> array);
array<bool, 32> decimalToBinaryArray(int decimal);
void readFile();
void display();
void displayMemoryLine(array<bool, 32> memoryLine);

string filename = "testfile.txt";
vector<std::array<bool, 32>> memory;
array<bool, 32> PI;
array<bool, 32> accumulator;
int CI = 0;
int opcode = 0;
int operand = 0;
bool stop = false;
bool isStepByStep = false;

int main(int argc, char *argv[]) {
	if (argc == 2) {
		filename = argv[1];
		cout << "Filename set to " << filename << endl;
	}
	readFile();
	cout << "Go step by step? [Y/n]" << endl;
	string input;
	cin >> input;
	if (input == "Y" || input == "y") {
		isStepByStep = true;
	}
	while (CI < 31 && CI < (int)memory.size() - 1) {
		CI++;
		fetch();
		decode();
		execute();
		display();
		if (stop) {
			return 0;
		}
		if (isStepByStep) {
			cout << "Enter anything to go to the next step or STOP to stop" << endl;
			cin >> input;
			if (input == "STOP") {
				return 0;
			}
		}
	}
	return 0;
}

// Fetching memory line at number stored in CI (Control Instruction)
// to PI (Present Instruction)
void fetch() {
	PI = memory[CI];
}

// Decode operand and opcode from PI (Present Instruction)
void decode() {
	vector<bool> operandBinary;
	vector<bool> opcodeBinary;
	int i = 0;
    // Get binary format of operand
	for (; i < 13; i++) {
		operandBinary.push_back(PI[i]);
	}
    // Get binary format of opcode
	for (; i < 17; i++) {
		opcodeBinary.push_back(PI[i]);
	}
	operand = binaryToDecimal(operandBinary);
	opcode = binaryToDecimal(opcodeBinary);
	cout << endl;
	cout << "Decoded operand " << operand << " and opcode " << opcode;
}

// Converting binary number (stored in a vector in big endian format) to decimal number
int binaryToDecimal(vector<bool> binaryVector) {
	int decimal = 0;
	for (size_t i = 0; i < binaryVector.size(); i++) {
		decimal += binaryVector[i] * pow(2, (int)i);
	}
	return decimal;
}

// Converting binary number (stored in a vector in big endian format) to decimal number
int binaryToDecimalArray(array<bool, 32> array) {
	int decimal = 0;
	for (size_t i = 0; i < array.size() - 1; i++) {
		decimal += array[i] * pow(2, (int)i);
	}
    // Check sign of binary number
	if (array[array.size() - 1]) {
		decimal *= -1;
	}
	return decimal;
}

// Cnverting decimal number to binary number stored as array
array<bool, 32> decimalToBinaryArray(int decimal) {
	bool isNegative = decimal < 0;
	array<bool, 32> array;
	if (isNegative) {
		decimal *= -1;
	}
	for (int i = 30; i >= 0; i--) {
		// If decimal is smaller than 2^i
        if (decimal < pow(2, i)) {
			array[i] = false;
		} else {
			array[i] = true;
			decimal -= pow(2, i);
		}
	}
	array[31] = isNegative;
	return array;
}

// Reading input file and storing it's contents in memory
void readFile() {
    string line;
    array<bool, 32> memoryLine;
    ifstream file(filename);
    if (!file) {
        // error opening file
        cout << "File error" << endl;
    }
    while(getline(file, line)) {
        cout << line << endl;
        if (line.size() >= 32) {
            // convert string to bool array
            for (size_t i = 0; i < 32; i++) {
                if (line[i] == '0') {
                    memoryLine[i] = 0;
                } else if (line[i] == '1') {
                    memoryLine[i] = 1;
                } else {
                    cout << "input file not in binary" << endl;
                }
            }
            // push instruction to memory
            memory.push_back(memoryLine);
        }
    }
    array<bool, 32> empty = {0};
    while (memory.size() < 32) {
    	memory.push_back(empty);
    }
    file.close();
}

// Displaying memory, CI, PI and accumulator
void display() {
    cout << endl;
    // Display memory
    cout << "Memory:" << endl;
    for (size_t i = 0; i < memory.size(); i++) {
        // Display number of the memory line
        cout << i << ":\t ";
        displayMemoryLine(memory[i]);
        cout << endl;
    }
    cout << "CI: " << CI << endl;
    cout << "PI: " << endl;
    displayMemoryLine(PI);
    cout << endl;
    cout << "Accumulator: " << endl;
    displayMemoryLine(accumulator);
    cout << endl;
}

// Displaying a line of memory
void displayMemoryLine(array<bool, 32> memoryLine) {
    char trueChar = '1';
    char falseChar = '0';
    for (int i = 0; i < (int)memoryLine.size(); i++) {
        // Split display of a memory line to groups of 4 bools (for visibility)
        if ( i >= 4 && i % 4 == 0) {
            cout << " ";
        }
        // if bit is "true"
        if (memoryLine[i]) {
            cout << trueChar;
        } else {
            cout << falseChar;
        }
        cout << " ";
    }
    cout << "Value: " << binaryToDecimalArray(memoryLine);
}

// Executing operations
void execute() {
	cout << endl;
    switch (opcode) {
	    case 0:
	        // set CI to content of memeory location
	        CI = binaryToDecimalArray(memory[operand]);
	        cout << "JMP";
	        break;
	    case 1:
	        // add content at memory location to CI
	        CI = CI + binaryToDecimalArray(memory[operand]);
	        cout << "JRP";
	        break;
	    case 2:
	        // load accumulator with negavive content at memory location
	        accumulator = memory[operand];
	        accumulator[31] = !accumulator[31];
	        cout << "LND";
	        break;
	    case 3:
	        // add accumulator content to memory location
	        memory[operand] = accumulator;
	        cout << "STO";
	        break;
	    case 4:
	    case 5:
	        // subtract content at memory location from accumulator
	        accumulator = decimalToBinaryArray(binaryToDecimalArray(accumulator) - binaryToDecimalArray(memory[operand]));
	        cout << "SUB";
	        break;
	    case 6:
	        // increment CI if accumulator is negative
	        if (binaryToDecimalArray(accumulator) < 0)
	            CI++;
	        cout << "CMP";
	        break;
	    case 7:
	        // set off stop lamp and half machine
	    	stop = true;
	    	cout << "STP";
	        break;
        case 8:
	        // copy variable to accumulator
            accumulator = memory[operand];
	    	cout << "LDP" << endl;
	        break;
        case 9:
	        // multiply variales
            accumulator = decimalToBinaryArray(binaryToDecimalArray(accumulator) * binaryToDecimalArray(memory[operand]));
	    	cout << "MTP" << endl;
	        break;    
	    default:
	        break;
    }
}
