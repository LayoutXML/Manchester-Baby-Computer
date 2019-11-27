/*
	Names: Calum Logan, Emilija Budryte, Rokas Jankunas, Jokubas Butkus, & Momchil Badzhev
	assembler.cpp written by: Calum Logan, Emilija Budryte
	Matriculation Numbers: 180013466, ###ADD MATRICULATION NUMBERS
	Module Code: AC21008
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

vector <string> temp; // temporary variables in binary

//variables
string inputFile = "input.txt";	//Assembly language text file is read in
string outputFile = "output.txt";	//Machine code (binary) text file is produced
bool started = false;	//Boolean to flag the START: keyword for the first pass
bool ended = false;		//Boolean to flag the END: keyword for the first pass
bool finishFirstPass = false;	//Boolean to mark when the first pass has ended
int lines = 0; //counts the lines in the assembly program code
int count = 0;

//functions
bool firstPass();	//Parses the file once, saving symbols to the symbol table
bool secondPass();	//Second pass will save variable information at the end of the program
void display();	//displays every symbol in the symbol table
bool instructionSet(); //7 basic instructions
string addZeros(int amount);
string convertToBinary(string num);
string convertWithZeros(string num);
void convertToMachineCode();

int main() 
{
	if (firstPass()) 
	{
		instructionSet();
		secondPass();
		convertToMachineCode();
		display();
		//printToFile();
	} 
	else 
	{
		return 1;
	}
	return 0;
}

bool firstPass()
{
	ifstream file (inputFile);
	string line;	//each line of the input file
	string label = "";	//the label of a symbol read in the file
	string address = "";	//the address of a symbol read in the file
	bool readLabel = false;	//boolean which tells us a label is being read
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

	        	if (line[i] == ' ')	//if we read a space:
	        	{
	        		if (label != "")	//if the label variable for this line isn't empty (contains a label), we have already read the label so we set its boolean to true
	        		{
	        			readLabel = true;
	        		}
	        		if (address != "")	//if the address variable for this line isn't empty (contains an address), we have already read the address os we set its boolean to true
	        		{
	        			readAddress = true;
	        		}
	        		else 
	        		{
	        			continue;	//otherwise if we read a space but already have a label and address, we just continue until we reach the end line character.
	        		}
	        	}
	        	else	//if we read a character that is anything but a space or an end-of-line character, we need to save it somewhere 
	        	{ 
	        		if (readLabel == false)	//if we haven't read any labels and havent reached the end of the program, we add currently read characters to the label variable
	        		{
	        			label += line[i];	//add the current character to the label string

	        			if (label == "START:")	//if we read the 'START:' flag, we set the started boolean to true and reset the label string then move on to the next character.
	        			{
	        				started = true;
	        				label = "";
	        				continue;
	        			}
	        			else if (label == "END:")	//Same as above for the end.
	        			{
	        				ended = true;
	        				label = "";
	        				continue;
	        			}
	        		}
	        		else if (readAddress == false && ended == false)	// if we haven't read an address and haven't reached the end of the program, we add currently read characters to the address variable
	        		{
	        			address += line[i];
	        		}
	        	}
	        }
	        //At this point, we should have a label and address to save to the Symbol table
	       	if (label != "" && started == true)	// If we have information ready to save and we're still in the program section for the first pass
	       	{
	       		if (ended == true)
	       		{
	       			Symbol newSymbol;	
					newSymbol.label = label;
					newSymbol.address = "";
					symbolTable.push_back(newSymbol);
	       		}
	     		else 
	     		{
		     		Symbol newSymbol;	//create a new symbol, set its variables and push on to the symbol table
					newSymbol.label = label;
					newSymbol.address = address;
					symbolTable.push_back(newSymbol);
	     		}
	       		lines++;
				//cout << "LABEL - " << label << endl;
	       		//cout << "ADDRESS - " << address << endl;
	       	}

	       	if (started == true && ended == true)	//If we have reached the end section of the program, we break out of the loop. This is the end of the first pass.
	       	{
	       		break;
	       	}

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
			firstBuffer.push_back("000");
		}
		else if (symbolTable.at(i).label == "JRP" ) {
			firstBuffer.push_back("100");
		}
		else if (symbolTable.at(i).label == "LDN" ) {
			firstBuffer.push_back("010");
		}
		else if (symbolTable.at(i).label == "STO" ) {
			firstBuffer.push_back("110");	
		}
		else if (symbolTable.at(i).label == "SUB" ) {
			firstBuffer.push_back("001");
		}
		else if (symbolTable.at(i).label == "SUB" ) {
			firstBuffer.push_back("101");
		}
		else if (symbolTable.at(i).label == "CMP" ) {
			firstBuffer.push_back("011");	
		}
		else if (symbolTable.at(i).label == "STP" ) {
			firstBuffer.push_back("111");	
		}
		else {
			cout << "Error, could not find an instruction: " << symbolTable.at(i).label  << "." << endl;
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
	
	bool variableIsValid = false;
	bool readName;
   	bool readValue;
	
	string name;
	string value;
	int lineNum = 0;

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
        	{
        		break;
        	}
        	if (line[i] == ' ')	//if we read a space:
        	{
        		if (name != "" )	//if the label variable for this line isn't empty (contains a label), we have already read the label so we set its boolean to true
        		{
        			readName = true;
        		}
        		if (value != "")	//if the address variable for this line isn't empty (contains an address), we have already read the address os we set its boolean to true
        		{
        			readValue = true ;
        		}
        		else 
        		{
        			continue;	//otherwise if we read a space but already have a label and address, we just continue until we reach the end line character.
        		}
        	}
        	else	//if we read a character that is anything but a space or an end-of-line character, we need to save it somewhere 
        	{ 
        		if (readName == false)	//if we haven't read any labels and havent reached the end of the program, we add currently read characters to the label variable
        		{
        			if (line[i] != ':')
        			{
        				name += line[i];	//add the current character to the label string
        			}
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
        		}
        		else if (readValue == false)	
        		{
        			if(line[i] != ':')
        			{
        				value += line[i];
        			}
        			if (value == "VAR")	
        			{
        				value = "";
        				continue;
        			}
        		}
	        }
	    }
       	if (name != "" && value != "" && ended == true)	
       	{
       		lines++;
       		
       		string value1 = convertWithZeros(value);
       		temp.push_back(value1);
			
			Operand newOperand;	
			newOperand.name = name;
			newOperand.value = value1;
			newOperand.lineNum = lines;
			operandTable.push_back(newOperand);
		}
	}
	file.close();
	return true;
}

void convertToMachineCode()
{
	for (int i = 0; i < (int)symbolTable.size(); i++)
	{
		string stringMaster = "";

		if (symbolTable.at(i).address == "")
		{
			stringMaster += addZeros(13);
		}
		else 
		{
			for(int j = 0; j < (int)operandTable.size(); j++)
			{
				if (symbolTable.at(i).address == operandTable.at(j).name)
				{
					string s = to_string(operandTable.at(j).lineNum);
					string result = convertToBinary(s);
					stringMaster = stringMaster + result;
				}
			}
		}
		stringMaster = stringMaster + firstBuffer.at(count) + addZeros(16);
		outputBuffer.push_back(stringMaster);
		count++;
	}

	for (int i = 0; i < (int)temp.size(); i++)
	{
		outputBuffer.push_back(temp.at(i));
	}

}

string addZeros(int amount)
{
	string returnString = "";
	for (int i = 0; i < amount; i++)
	{
		returnString += "0";
	}
	return returnString;
}


//only for variables but not for line number++
string convertToBinary(string num)
{
    int binaryNumber[13];

    string result;
    
    int a = 0;
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

	for (int a = elements; a < 13; a++)
    	binaryNumber[a] = 0;

    for (int j = 0; j < 13; j++) 
    	result = result + to_string(binaryNumber[j]);
    
    return result;
}

string convertWithZeros(string num)
{
	int binaryNumber[32];

    string result;
    
    int a = 0;
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
	
	for (int a = elements; a < 32; a++)
    	binaryNumber[a] = 0;
    
    for (int j = 0; j < 32; j++) 
    	result = result + to_string(binaryNumber[j]);

    return result;
}

void display()
{
	for (int i = 0; i < (int)symbolTable.size(); i++)	//For each Symbol in the symbol table, print its lavel and address
	{
		cout << "Label: " << symbolTable.at(i).label << endl;
		cout << "Address: " << symbolTable.at(i).address << endl;
	}
	
	cout << "Lines: " << lines << endl;
	
	/*for (int i = 0; i < (int)firstBuffer.size(); i++)	//For each Symbol in the symbol table, print its lavel and address
	{
		cout << "Set: " << firstBuffer.at(i) << endl;
	}
	
	for (int i = 0; i < (int)operandTable.size(); i++)
	{
		cout << "Var name: " << operandTable.at(i).name << endl;
		cout << "Var value: " << operandTable.at(i).value << endl;
		cout << "Var line: " << operandTable.at(i).lineNum << endl;
	}*/
	
	for (int i = 0; i < (int)outputBuffer.size(); i++)
	{
		cout << outputBuffer.at(i) << endl;
	}
}
