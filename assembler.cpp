/*
	Names: Calum Logan, Emilija Budryte, Rokas Jankunas, Jokubas Butkus, & Momchil Badzhev
	assembler.cpp written by: Calum Logan & Emilija Budryte
	Matriculation Numbers: 180013466, 180017115, 180003228, 180017346, 180012872
	Module Code: AC21009

	Run with provided assembly source code files: 
	AddTwoNumbers.txt (equivalent to provided input.txt), AddThreeNumbers.txt, MultiplyTwoNumbers.txt, Fibonacci.txt
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct Symbol {		//Symbol structure for the Symbol table - each symbol contains a label and a memory address
	string label;
	string address;
};

struct Operand { 	// Operand structure for the operand table
	string name;
	string value;
	int lineNum;
};

vector <Symbol> symbolTable;
vector <Operand> operandTable;
vector <string> outputBuffer;
vector <string> firstBuffer;
vector <string> temp; 			// temporary variables in binary code

//variables
string inputFile = "input.txt";		// Assembly language text file is read in
string outputFile = "output.txt";	// Machine code (binary) text file is produced

bool started = false;	// Boolean to flag the START: keyword 
bool ended = false;		// Boolean to flag the END: keyword 
bool ending = false;	// Boolean to read the line where END: is

int lines = 0; 				// counts the lines in the assembly program code
int count = 0;				// counts the items in a first buffer
int varsBeforeStart = 0;	// counts the amount of variables declared before the start (usually 1)

//functions
bool firstPass();						// Parses the file once, saving symbols to the symbol table
bool secondPass();						// Second pass will save variable information
bool instructionSet(string label); 		// 7 main instructions
string convertToBinary(string num);		// converting a variable to a binary code
string convertToBinary2(int number);	// converting a line number to a binary code
string addZeros(int amount, int chars);	// add zeros
bool convertToMachineCode();			// convert 
void printToFile();						// printing machine code to a file

int main() 
{
	int choice;
	string fn;

	cout << "-------------------------------------------------------" << endl;
	cout << "Choose a file you want run or enter your own file name." << endl;
	cout << "1. AddTwoNums.txt " << endl;
	cout << "2. AddThreeNums.txt" << endl;
	cout << "3. MultiplyTwoNums.txt" << endl;
	cout << "4. Fibonacci.txt" << endl;
	cout << "5. Enter your file name." << endl;
	cout << "0. Exit." << endl;
	cout << "-------------------------------------------------------" << endl;
	cin >> choice;

	switch(choice) 
	{
		case 1:
			inputFile = "AddTwoNumbers.txt";
			break;
		case 2:
			inputFile = "AddThreeNumbers.txt";
			break;
		case 3:
			inputFile = "MultiplyTwoNumbers.txt";
			break;
		case 4:
			inputFile = "Fibonacci.txt";
			break;
		case 5:
			cin >> fn;
			inputFile = fn;
			break;
		case 0:
			exit(0);
			break;
		default:
			cout << "Please select a valid option." << endl;
			break;
	}

	cout << endl;

	if (firstPass()) 
	{
		if (!secondPass())
			return 1;
		else 
		{
			if(!convertToMachineCode())
				return 1;
			else
				printToFile();
		}
	} 
	else 
		return 1;
	return 0;
}

bool instructionSet(string label)
{
	if (label == "JMP") {
		cout << "Instruction JMP was found and added to the output buffer." << endl;
		firstBuffer.push_back("000");
	} else if (label == "JRP") {
		cout << "Instruction JRP was found and added to the output buffer." << endl;
		firstBuffer.push_back("100");
	} else if (label == "LDN") {
		cout << "Instruction LDN was found and added to the output buffer." << endl;
		firstBuffer.push_back("010");
	} else if (label == "STO") {
		cout << "Instruction STO was found and added to the output buffer." << endl;
		firstBuffer.push_back("110");	
	} else if (label == "SUB") {
		cout << "Instruction SUB was found and added to the output buffer." << endl;
		firstBuffer.push_back("001");
	} else if (label == "SUB") {
		cout << "Instruction SUB was found and added to the output buffer." << endl;
		firstBuffer.push_back("101");
	} else if (label == "CMP") {
		cout << "Instruction CMP was found and added to the output buffer." << endl;
		firstBuffer.push_back("011");	
	} else if (label == "STP") {
		cout << "Instruction STP was found and added to the output buffer." << endl;
		firstBuffer.push_back("111");	
	} else if (label == "LDP") {
		cout << "Instruction LDP was found and added to the output buffer." << endl;
		firstBuffer.push_back("0001");	
	} else if (label == "MTP") {
		cout << "Instruction MTP was found and added to the output buffer." << endl;
		firstBuffer.push_back("1001");	
	} else {
		cout << "Error, could not find an instruction: " << label << "." << endl;
		return false;
	}
	return true;
}

bool firstPass()
{
	ifstream file (inputFile);
	string line;				//each line of the input file
	string label = "";			//the label of a symbol read in the file
	string address = "";		//the address of a symbol read in the file
	bool readLabel = false;		//boolean which tells us a label is being read
	bool readAddress = false;	//boolean that tells us an address is being read

    if (!file)
  	{
        cout << "Error, cannot open a file." << endl;
        return false;
    }
    else
    {
        while (getline(file, line))	//while a line can be read, we go on to the next line
        {
        	//reset relevant variables
        	label = "";	
        	address = "";
        	readLabel = false;
        	readAddress = false;

        	for (int i = 0; i < (int)line.length(); i++)	
        	{
	        	if (line[i] == ';')	//once we hit a ';' (end of line) we skip as we can ignore anything next to it on the same line as a comment
	        		break;
	        	if (line[i] == ' ' || line[i] == '\t') 
	        	{	
	        		if (label != "") 
	        		{ 
	        			readLabel = true;
	        		}
	        		else if (address != "")	
	        		{
	        			readAddress = true;
	        		}
	        		else 
	        			continue;	
	        	}
	        	else	
	        	{ 
	        		if (readLabel == false)	
	        		{
	        			label += line[i];	

	        			if (label == "START:")
	        			{
	        				cout << "Label START was found in the code." << endl;
	        				started = true;
	        				label = "";
	        				continue;
	        			}
	        			else if (label == "END:")	
	        			{
	        				cout << "Label END was found in the code." << endl;
	        				ended = true;
	        				label = "";
	        				continue;
	        			}
	        		}
	        		else if (readAddress == false)	// if we haven't read an address and haven't reached the end of the program, we add currently read characters to the address variable
	        			address += line[i];
	        	}

	        }
	       	if (label != "" && started == true)	// if we find a label we store it in the symbol table
	       	{
       			Symbol newSymbol;	
				newSymbol.label = label;
				newSymbol.address = address;
				symbolTable.push_back(newSymbol);
				if (!instructionSet(label))
					return false;
	       		lines++;
	       	}

	       	if (started == true && ended == true)	
	       		break;
	    }
	    if((started == false && ended == true) || (started == true && ended == false) || (started == false && ended == false))
	    {
	    	cout << "Error, please include START: and END: flags in your program." << endl;
	    	file.close();
	    	return false;
	    }
    }
    file.close();
    cout << endl;
	return true;
}

bool secondPass()
{
	ifstream file (inputFile);
	ended = false;
	started = false;
	ending = false;
	
	bool readName;
   	bool readValue;
   	bool isValidInstruction = false;
	
	string name;
	string value;
	string line;	
	
	while(getline(file, line))
	{
		name = "";
		value = "";
    	readName = false;
    	readValue = false;
    	isValidInstruction = false;

    	for (int i = 0; i < (int)line.length(); i++)	
    	{
        	if (line[i] == ';')	//once we hit a ';' (end of line) we skip as we can ignore anything next to it on the same line as a comment
        		break;
        	if (line[i] == ' ' || line[i] == '\t')	
        	{
        		if (name != "")	
        		{
        			readName = true;
        		}
        		else if (value != "")	
        		{
        			readValue = true;
        		}
        		else 
        			continue;	
        	}
        	else	
        	{ 
        		if (readName == false)	//if we haven't read any labels and havent reached the end of the program, we add currently read characters to the label variable
        		{
        			if (line[i] != ':')
        				name += line[i];	
        			if (name == "START")	
        			{
        				started = true;
        				name = "";
        				continue;
        			}
        			else if (name == "END")	
        			{
        				ended = true;
        				ending = true;
        				name = "";
        				continue;
        			}
        			else
        				continue;
        		}
        		else if (readValue == false)	
        		{
        			if(line[i] != ':')
        				value += line[i];
        			if (value == "VAR")	
        			{
        				value = "";
        				continue;
        			}
        			else
        				continue;
        		}
	        }
	    }

	    for (int i = 0; i < (int)symbolTable.size(); i++)
	    {
	    	if(name == symbolTable.at(i).label)
	    		isValidInstruction = true;
	    }

	    //Storing a variable if it's declared after the END flag
       	if (name != "" && value != "" && ended == true && ending == false )	
       	{
       		lines++;
       		string convertedValue = convertToBinary(value);	
       		temp.push_back(convertedValue);
			cout << "New variable " << name << " was found, converted to binary code and stored in the table." << endl;
			
			Operand newOperand;	
			newOperand.name = name;
			newOperand.value = convertedValue;
			newOperand.lineNum = lines;
			operandTable.push_back(newOperand);
		}

		//Storing a variable if it's declared before the START flag and its name is VAR
		if (started == false && ended == false && name == "VAR" && value != "")
		{
			varsBeforeStart++;
			string convertedValue = convertToBinary(value);
			temp.push_back(convertedValue);
			cout << "New variable " << name << " was found, converted to binary code and stored in the table." << endl;
		}

		//Storing a variable if it's declared before the START flag and has a name other than VAR
		if (started == false && ended == false && name != "VAR" && value != "")	
       	{
       		string convertedValue = convertToBinary(value);
       		temp.push_back(convertedValue);
			cout << "New variable " << name << " was found, converted to binary code and stored in the table." << endl;
			
			Operand newOperand;	
			newOperand.name = name;
			newOperand.value = convertedValue;
			newOperand.lineNum = varsBeforeStart;
			operandTable.push_back(newOperand);
			varsBeforeStart++;
		}

		//Checks if the variable has a valid value
		if (name != "VAR" && isValidInstruction == false && name != "" && value == "")
		{
			cout << "The variable " << name << " needs to have a valid value." << endl;
			return false;
		}
		if(ending)
			ending = false;
	}
	file.close();
	cout << endl;
	return true;
}

bool convertToMachineCode()
{
	bool valid = false;

	//pushes every variable declared before main program to output buffer
	for(int i = 0; i < varsBeforeStart; i++)
		outputBuffer.push_back(temp.at(i));

	//Loops through the symbol table and pushes contents to the output buffer
	for (int i = 0; i < (int)symbolTable.size(); i++)
	{
		valid = false;
		string stringMaster = "";

		//Checks if the symbol table address is empty
		if (symbolTable.at(i).address == "")
		{
			cout << symbolTable.at(i).label  << " address is empty." << endl;
			for (int a = 0; a < 13; a++)
				stringMaster += "0";
		}
		else 
		{
			//If not empty, it loops through operand table and looks for a match
			for(int j = 0; j < (int)operandTable.size(); j++)
			{
				if (symbolTable.at(i).address == operandTable.at(j).name)
				{					
					stringMaster = stringMaster + convertToBinary2(operandTable.at(j).lineNum);
					valid = true;
					cout << symbolTable.at(i).label << " address was found and matched with a variable." << endl;
				}
			}
			//If no match is found, prints message
			if (valid == false)
			{
				cout << "Sorry, cannot find the variable at address " << symbolTable.at(i).address << "." << endl;
				return false;
			}
		}
		int chars = (int)firstBuffer.at(count).size();
		stringMaster = stringMaster + firstBuffer.at(count) + addZeros(19,chars);
		outputBuffer.push_back(stringMaster);
		cout << "New line in the output buffer was created." << endl;
		count++;
	}

	//Adds variables declared after the END flag to the output buffer
	for (int i = varsBeforeStart; i < (int)temp.size(); i++)
		outputBuffer.push_back(temp.at(i));
	return true;
}

string addZeros(int amount, int chars)
{
	string returnString = "";
	for(int i = 0 + chars; i < amount; i++)
		returnString += "0";
	return returnString;
}

// converting line number to a binary code
string convertToBinary2(int number)
{
	int binaryNumber[13];
	string result; 
    int i = 0;
    int elements = 0;

	while (number > 0) {
        binaryNumber[i] = number % 2;
        number = number / 2;
        i++;
        elements++;
    }

	for (int a = elements; a < 13; a++) 
    	binaryNumber[a] = 0;
	
    for (int j = 0; j < 13; j++) 
    	result = result + to_string(binaryNumber[j]);

    return result;
}

// converting variables to a binary code
string convertToBinary(string num)
{
    int binaryNumber[32];
    string result;
    int i = 0;
    int elements = 0;
    int number;

    number = stoi(num);

    while (number > 0) {
        binaryNumber[i] = number % 2;
        number = number / 2;
        i++;
        elements++;
    }

    //for example, if we have a number 1025, its binary code is 10000000001, but we need 32 digits
	for (int a = elements; a < 32; a++)
    	binaryNumber[a] = 0;
    
    for (int j = 0; j < 32; j++) 
    	result = result + to_string(binaryNumber[j]);

    return result;
}

void printToFile()
{
	if(outputBuffer.size() > 32)	//If the output buffer is more than 32 lines long, it won't work in the simulator so prints an error and never saves to file
	{
		cout << "Error, the Machine code produced from the Assembly Code source file was too long!" << endl;
		cout << "Please ensure your source code is 32 lines long or less" << endl;
	}
	else 	//Saves the contents of the output buffer to file
	{
		ofstream out (outputFile);
		for (int i = 0; i < (int)outputBuffer.size(); i++)
			out << outputBuffer.at(i) << endl;
		cout << endl << "Machine code printed to output.txt successfully." << endl << endl;
		out.close();
	}
}
