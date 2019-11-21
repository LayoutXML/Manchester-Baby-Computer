#include <iostream>
#include <vector>
#include <array>

using namespace std;

vector<std::array<bool, 32>> memory;

int main() {
	array<bool, 32> test;
	memory.push_back(test);
	cout << memory.size() << endl;
	return 0;
}