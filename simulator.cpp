#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <cmath>

using namespace std;

void decode();
void fetch();
int binaryToDecimal(vector<bool> binaryVector);
void readFile();
void display();
void displayMemoryLine(array<bool, 32> memoryLine);

string filename = "testfile.txt";
vector<std::array<bool, 32>> memory;
array<bool, 32> PI;
array<bool, 32> accumulator;
int CI = 0;
int opcode;
int operand;

int main() {
	readFile();

	while (CI < 31 && CI < (int)memory.size() - 1) {
		CI++;
		fetch();
		decode();
		// execute();
		display();
	}
	return 0;
}

void fetch() {
	PI = memory[CI];
}

void decode() {
	vector<bool> operandBinary;
	vector<bool> opcodeBinary;
	int i = 0;
	for (i; i < 13; i++) {
		operandBinary.push_back(PI[i]);
	}
	for (i; i < 16; i++) {
		opcodeBinary.push_back(PI[i]);
	}
	operand = binaryToDecimal(operandBinary);
	opcode = binaryToDecimal(opcodeBinary);
}

//Converting binary number (stored in a vector in big endian format) to decimal number
int binaryToDecimal(vector<bool> binaryVector) {
	int decimal = 0;
	for (size_t i = 0; i < binaryVector.size(); i++) {
		decimal += binaryVector[i] * pow(2, (int)i);
	}
	return decimal;
}

// read file, store instructions in memory
void readFile() {
    string line;
    array<bool, 32> memoryLine;
    ifstream file;
    file.open(filename);
    if (!file) {
        // error opening file
    }
    while(getline(file, line) && memory.size() <= 32) {
        if (line.size() == 32) {
            // convert string to bool array
            for (size_t i = 0; i < line.length(); i++) {
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
    file.close();
}

// Display memory, CI, PI and accumulator
void display() {
    char trueChar = '1';
    char falseChar = '0';
    // Memory display
    cout << "Memory:" << endl;
    for (size_t i = 0; i < memory.size(); i++) {
        cout << i << ": ";
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

void displayMemoryLine(array<bool, 32> memoryLine) {
    char trueChar = '1';
    char falseChar = '0';
    for (size_t i = 0; i < memoryLine.size(); i++) {
        // if bit is "true"
        if (memoryLine[i]) {
            cout << trueChar;
        } else {
            cout << falseChar;
        }
        cout << " ";
    }
}

