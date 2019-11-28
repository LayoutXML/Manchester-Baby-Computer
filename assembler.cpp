/*
	Names: Calum Logan, Emilija Budryte, Rokas Jankunas, Jokubas Butkus, & Momchil Badzhev
	assembler.cpp written by: Calum Logan, Emilija Budryte
	Matriculation Numbers: 180013466, ###ADD MATRICULATION NUMBERS
	Module Code: AC21009
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

struct Operand {
	string name;
	string value;
	int lineNum;
};

vector <Symbol> symbolTable;	//symbol table we make use of whenever a symbol is seen in the assembly code
vector <Operand> operandTable;

vector <string> outputBuffer;	//binary
vector <string> firstBuffer;

vector <string> temp; 			// temporary variables in binary

//variables
string inputFile = "Multiply2No.txt";		//Assembly language text file is read in
string outputFile = "output.txt";	//Machine code (binary) text file is produced
bool started = false;				//Boolean to flag the START: keyword for the first pass
bool ended = false;					//Boolean to flag the END: keyword for the first pass
bool finishFirstPass = false;		//Boolean to mark when the first pass has ended
int lines = 0; 						//counts the lines in the assembly program code
int count = 0;
int varsBeforeStart = 0;			//counts the amount of variables declared before the start (usually 1)

//functions
bool firstPass();					//Parses the file once, saving symbols to the symbol table
bool secondPass();					//Second pass will save variable information at the end of the program
void display();						//displays every symbol in the symbol table
bool instructionSet(); 				//7 basic instructions
string convertToBinary(string num);
string convertToBinary2(int number);
string addZeros(int amount, int chars);
bool convertToMachineCode();
void printToFile();

int main() 
{
	int choice;
	string fn;

	cout << "-------------------------------------------------------" << endl;
	cout << "Choose a file you want run or enter your own file name." << endl;
	cout << "1. AddTwoNo.txt " << endl;
	cout << "2. AddThreeNo.txt" << endl;
	cout << "3. Multiply2No.txt" << endl;
	cout << "4. Enter your file name." << endl;
	cout << "0. Exit." << endl;
	cout << "-------------------------------------------------------" << endl;
	cin >> choice;

	switch(choice) 
	{
		case 1:
			inputFile = "AddTwoNo.txt";
			break;
		case 2:
			inputFile = "AddThreeNo.txt";
			break;
		case 3:
			inputFile = "Multiply2No.txt";
			break;
		case 4:
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
		instructionSet();
		if (secondPass() == false)
			return 1;
		else 
		{
			if(convertToMachineCode() == false)
				return 1;
			else
				printToFile();
		}
	} 
	else 
		return 1;
	return 0;
}

bool firstPass()
{
	ifstream file (inputFile);
	string line;				//each line of the input file
	string label = "";			//the label of a symbol read in the file
	string address = "";		//the address of a symbol read in the file
	bool readLabel = false;		//boolean which tells us a label is being read
	bool readAddress = false;	//boolean that tells us an address is being read

	//if we can't open the file, simply print an error message
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

        	for (int i = 0; i < (int)line.length(); i++)	//for each character in the line
        	{
	        	if (line[i] == ';')	//once we hit a ';' (end of line) we skip as we can ignore anything next to it on the same line as a comment
	        		break;
	        	if (line[i] == ' ' || line[i] == '\t')	//if we read a space:
	        	{ 
	        		if (label != "")	//if the label variable for this line isn't empty (contains a label), we have already read the label so we set its boolean to true
	        			readLabel = true;
	        		if (address != "")	//if the address variable for this line isn't empty (contains an address), we have already read the address os we set its boolean to true
	        			readAddress = true;
	        		else 
	        			continue;	//otherwise if we read a space but already have a label and address, we just continue until we reach the end line character.
	        	}
	        	else	//if we read a character that is anything but a space or an end-of-line character, we need to save it somewhere 
	        	{ 
	        		if (readLabel == false)	//if we haven't read any labels and havent reached the end of the program, we add currently read characters to the label variable
	        		{
	        			label += line[i];	//add the current character to the label string

	        			if (label == "START:")	//if we read the 'START:' flag, we set the started boolean to true and reset the label string then move on to the next character.
	        			{
	        				cout << "Label START was found in the code." << endl;
	        				started = true;
	        				label = "";
	        				continue;
	        			}
	        			else if (label == "END:")	//Same as above for the end.
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
	        //At this point, we should have a label and address to save to the Symbol table
	       	if (label != "" && started == true)	// If we have information ready to save and we're still in the program section for the first pass
	       	{
       			Symbol newSymbol;	
				newSymbol.label = label;
				newSymbol.address = address;
				symbolTable.push_back(newSymbol);
	       		lines++;
	       	}

	       	if (started == true && ended == true)	//If we have reached the end section of the program, we break out of the loop. This is the end of the first pass.
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
	return true;
}

bool instructionSet()
{
	for (int i = 0; i < (int)symbolTable.size(); i++)
	{
		if (symbolTable.at(i).label == "JMP" ) {
			cout << "Instruction JMP was found and added to the output buffer." << endl;
			firstBuffer.push_back("000");
		}
		else if (symbolTable.at(i).label == "JRP" ) {
			cout << "Instruction JRP was found and added to the output buffer." << endl;
			firstBuffer.push_back("100");
		}
		else if (symbolTable.at(i).label == "LDN" ) {
			cout << "Instruction LDN was found and added to the output buffer." << endl;
			firstBuffer.push_back("010");
		}
		else if (symbolTable.at(i).label == "STO" ) {
			cout << "Instruction STO was found and added to the output buffer." << endl;
			firstBuffer.push_back("110");	
		}
		else if (symbolTable.at(i).label == "SUB" ) {
			cout << "Instruction SUB was found and added to the output buffer." << endl;
			firstBuffer.push_back("001");
		}
		else if (symbolTable.at(i).label == "SUB" ) {
			cout << "Instruction SUB was found and added to the output buffer." << endl;
			firstBuffer.push_back("101");
		}
		else if (symbolTable.at(i).label == "CMP" ) {
			cout << "Instruction CMP was found and added to the output buffer." << endl;
			firstBuffer.push_back("011");	
		}
		else if (symbolTable.at(i).label == "STP" ) {
			cout << "Instruction STP was found and added to the output buffer." << endl;
			firstBuffer.push_back("111");	
		}
		else if (symbolTable.at(i).label == "LDP" ) {
			cout << "Instruction LDP was found and added to the output buffer." << endl;
			firstBuffer.push_back("0001");	
		}
		else if (symbolTable.at(i).label == "MTP" ) {
			cout << "Instruction MTP was found and added to the output buffer." << endl;
			firstBuffer.push_back("1001");	
		}
		else {
			cout << "Error, could not find an instruction: " << symbolTable.at(i).label << "." << endl;
			return false;
		}
	}
	return true;
}

bool secondPass()
{
	ended = false;
	started = false;
	
	ifstream file (inputFile);
	
	bool readName;
   	bool readValue;
   	//bool isValidInstruction = false;
	
	string name;
	string value;

	string line;	//each line of the input file
	
	while(getline(file, line))
	{
		name = "";
		value = "";
    	readName = false;
    	readValue = false;

    	for (int i = 0; i < (int)line.length(); i++)	//for each character in the line
    	{
        	if (line[i] == ';')	//once we hit a ';' (end of line) we skip as we can ignore anything next to it on the same line as a comment
        		break;
        	if (line[i] == ' ' || line[i] == '\t')	//if we read a space:
        	{
        		if (name != "" )	//if the label variable for this line isn't empty (contains a label), we have already read the label so we set its boolean to true
        			readName = true;
        		if (value != "")	//if the address variable for this line isn't empty (contains an address), we have already read the address os we set its boolean to true
        			readValue = true ;
        		else 
        			continue;	//otherwise if we read a space but already have a label and address, we just continue until we reach the end line character.
        	}
        	else	//if we read a character that is anything but a space or an end-of-line character, we need to save it somewhere 
        	{ 
        		if (readName == false)	//if we haven't read any labels and havent reached the end of the program, we add currently read characters to the label variable
        		{
        			if (line[i] != ':')
        				name += line[i];	//add the current character to the label string
        			if (name == "START")	//if we read the 'START:' flag, we set the started boolean to true and reset the label string then move on to the next character.
        			{
        				started = true;
        				name = "";
        				continue;
        			}
        			else if (name == "END")	//Same as above for the end.
        			{
        				ended = true;
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

	    // for (int i = 0; i < (int)firstBuffer.size(); i++)
	    // {
	    // 	if(name == firstBuffer.at(i))
	    // 		isValidInstruction = true;
	    // }

	   	//cout << "Value!!!: " << value << "  name!!!: " << name << endl;

       	if (name != "" && value != "" && ended == true )	
       	{
       		lines++;
       		
       		cout << "Value: " << value << "  name: " << name << endl;
       		string convertedValue = convertToBinary(value);
       		temp.push_back(convertedValue);
			
			cout << "New variable " << name << " was found, converted to binary code and stored in the table." << endl;
			Operand newOperand;	
			newOperand.name = name;
			newOperand.value = convertedValue;
			newOperand.lineNum = lines;
			operandTable.push_back(newOperand);
		}
		if (started == false && ended == false && name == "VAR" && value != "")
		{
			varsBeforeStart++;
			cout << "Value no name : " << value << "  name: " << name << endl;
			string convertedValue = convertToBinary(value);
			temp.push_back(convertedValue);
		}
		if (started == false && ended == false && name != "VAR" && value != "")	
       	{
       		//lines++;
       		cout << "Value name before: " << value << "  name: " << name << endl;
       		string convertedValue = convertToBinary(value);
       		temp.push_back(convertedValue);
			
			Operand newOperand;	
			newOperand.name = name;
			newOperand.value = convertedValue;
			newOperand.lineNum = varsBeforeStart;
			operandTable.push_back(newOperand);
			varsBeforeStart++;
		}
		if (name != "VAR" && name != "STP" && name != "JMP" && name != "" && value == "")
		{
			cout << "VAR must have a valid value." << endl;
			return false;
		}

	}
	file.close();
	return true;
}

bool convertToMachineCode()
{
	bool valid = false;

	for(int i = 0; i < varsBeforeStart; i++)
		outputBuffer.push_back(temp.at(i));	//pushes every variable declared before main program to output buffer

	for (int i = 0; i < (int)symbolTable.size(); i++)
	{
		valid = false;
		string stringMaster = "";
		//cout << "symbol table address: " << symbolTable.at(i).address << endl;
		if (symbolTable.at(i).address == "")
		{
			cout << symbolTable.at(i).label  << " address is empty." << endl;
			for (int a = 0; a < 13; a++)
				stringMaster += "0";
		}
		else 
		{
			for(int j = 0; j < (int)operandTable.size(); j++)
			{
				if (symbolTable.at(i).address == operandTable.at(j).name)
				{					
					stringMaster = stringMaster + convertToBinary2(operandTable.at(j).lineNum);
					valid = true;
					cout << symbolTable.at(i).label << " address was found and matched with a variable." << endl;
				}
			}
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

//for line number
string convertToBinary2(int number)
{
	int binaryNumber[13];
	string result; 

    int i = 0;
    int elements = 0;

	while (number > 0)
    {
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

//only for variables
string convertToBinary(string num)
{
    int binaryNumber[32];
    string result;
    
    int i = 0;
    int elements = 0;
    int number;

    number = stoi(num);

    while (number > 0)
	{
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

void display()
{
	/*for (int i = 0; i < (int)symbolTable.size(); i++)	//For each Symbol in the symbol table, print its lavel and address
	{
		cout << "Label: " << symbolTable.at(i).label << endl;
		cout << "Address: " << symbolTable.at(i).address << endl;
	}
	
	cout << "Lines: " << lines << endl;
	
	for (int i = 0; i < (int)firstBuffer.size(); i++)	//For each Symbol in the symbol table, print its lavel and address
	{
		cout << "Set: " << firstBuffer.at(i) << endl;
	}*/
	
	for (int i = 0; i < (int)operandTable.size(); i++)
	{
		cout << "Var name: " << operandTable.at(i).name << endl;
		cout << "Var value: " << operandTable.at(i).value << endl;
		cout << "Var line: " << operandTable.at(i).lineNum << endl;
	}
	
	/*for (int i = 0; i < (int)outputBuffer.size(); i++)
	{
		cout << outputBuffer.at(i) << endl;
	}

	for (int i = 0; i < (int)temp.size(); i++)
	{
		cout << temp.at(i) << endl;
	}*/
}

void printToFile()
{
	if(outputBuffer.size() > 32)
	{
		cout << "ERROR!" << endl;
		cout << "The Machine code produced from the Assembly Code source file was too long!" << endl;
		cout << "Please ensure your source code is 32 lines long or less" << endl;
	}
	else
	{
		ofstream out (outputFile);
		for (int i = 0; i < (int)outputBuffer.size(); i++)
			out << outputBuffer.at(i) << endl;
		cout << "Machine code printed to output.txt successfully." << endl;
		out.close();
	}
	
}
