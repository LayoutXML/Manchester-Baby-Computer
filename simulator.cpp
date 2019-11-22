#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>

using namespace std;

void readFile();

string filename = "testfile.txt";
vector<std::array<bool, 32>> memory;

int main() {
	array<bool, 32> test;
	//memory.push_back(test);
	//cout << memory.size() << endl;
	
    readFile();
    cout << memory.size() << endl;

    for (size_t i = 0; i < memory.size(); i++) {
        for (int k = 0; k < 32; k++) {
            cout << memory[i][k];
        }
        cout << endl;
    }
    return 0;
}

// read file, store instructions in memory
void readFile() {
    string line;
    array<bool, 32> memoryLine;
    ifstream file;
    file.open("testfile.txt");
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
