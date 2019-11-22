#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <cmath>

using namespace std;

void decode();
void fetch();
void execute();
int binaryToDecimal(vector<bool> binaryVector);
void readFile();

string filename = "testfile.txt";
vector<std::array<bool, 32>> memory;
array<bool, 32> PI;
int CI = 0;
int opcode = 0;
int operand = 0;
int accumulator = 0;

int main() {
	readFile();

	while (CI < 31 && CI < (int)memory.size() - 1) {
		CI++;
		fetch();
		decode();
		execute();
		// display();
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
            // convert string to bool arraye
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

void execute(int operand, int opcode) {
    switch (opcode)
    {
    case 0:
        // set CI to content of memeory location
        CI = memory[operand];
        break;
    case 1:
        // add content at memory location to CI
        CI = CI + memory[operand];
        break;
    case 2:
        // load accumulator with negavive content at memory location
        accumulator = -memory[operand];
        break;
    case 3:
        // add accumulator content to memory location
        memory[operand] = accumulator;
        break;
    case 4:
        // subtract content at memory location from accumulator
        accumulator = accumulator - memory[operand];
        break;
    case 5:
        // !HELP! not sure what this should do
        break;
    case 6:
        // increment CI if accumulator is negative
        if (accumulator < 0)
            CI++;
        break;
    case 7:
        // set off stop lamp and half machine
        break;
    default:
        break;
    }
}
