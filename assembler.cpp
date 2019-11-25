/*
	Names: Calum Logan, Emilija Budryte, Rokas Jankunas, Jokubas Butkus, & Momchil Badzhev
	assembler.cpp written by: Calum Logan, Emilija Budryte
	Matriculation Numbers: 180013466, ###ADD MATRICULATION NUMBERS
	Module Code: AC21008
*/

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

struct Symbol {		//Symbol structure for the Symbol table - each symbol contains a label and a memory address
	string label;
	string address;
};

vector <Symbol> symbolTable;	//symbol table we make use of whenever a symbol is seen in the assembly code
vector <string> outputBuffer; //binary
vector <string> firstBuffer;
vector <string> varNames;

//variables
string inputFile = "input.txt";	//Assembly language text file is read in
string outputFile = "output.txt";	//Machine code (binary) text file is produced
bool started = false;	//Boolean to flag the START: keyword for the first pass
bool ended = false;		//Boolean to flag the END: keyword for the first pass
bool finishFirstPass = false;	//Boolean to mark when the first pass has ended
int lines = 1; //counts the lines in the assembly program code

//functions
bool firstPass();	//Parses the file once, saving symbols to the symbol table
bool secondPass();	//Second pass will save variable information at the end of the program
void display();	//displays every symbol in the symbol table
bool instructionSet(); //7 basic instructions

int main() {
	if (firstPass()) 
	{
		instructionSet();
		display();
		secondPass();
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
	        	{
	        		break;
	        	}
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
	        		if (readLabel == false && ended == false)	//if we haven't read any labels and havent reached the end of the program, we add currently read characters to the label variable
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
	        			}
	        		}
	        		else if (readAddress == false && ended == false)	// if we haven't read an address and haven't reached the end of the program, we add currently read characters to the address variable
	        		{
	        			address += line[i];
	        		}
	        	}
	        }
	        //At this point, we should have a label and address to save to the Symbol table
	       	if (label != "" && address != "" && started == true)	// If we have information ready to save and we're still in the program section for the first pass
	       	{
	       		if ( ended != true)	//And we haven't reached the end of the program yet
	       		{
					Symbol newSymbol;	//create a new symbol, set its variables and push on to the symbol table
					newSymbol.label = label;
					newSymbol.address = address;
					symbolTable.push_back(newSymbol);
	       		}
	       		else	//If we have reached the end of the program for the first pass, we'll only be saving the 'STP' label to make the program stop
	       		{
	       			address = "";	
	       			Symbol newSymbol;
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
	int a = 0;
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

bool  secondPass()
{
	ended = false;
	//reads each line:
	//after END:
		//saves variable names to a varNames vector
		//check each var name in symbol table matches this vector
	ifstream file (inputFile);
	bool readVarName;
	string varName;
	string line;	//each line of the input file
	while(getline(file, line))
	{
		varName = "";
		for (int i = 0; i < (int)line.length(); i++)	//for each character in the line
        {
        	if (line[i] == ';')	//once we hit a ';' (end of line) we skip as we can ignore anything next to it on the same line as a comment
	        {
	        	break;
	        }
	        if (line[i] == ' ')	//if we read a space:
	        {
	        	if (varName != "")	//if the varName isn't empty (contains a label), we have already read the name so we set its boolean to true
	        	{
	        		readVarName = true;
	        	}
	        }
	        // if (line[i] == ':')
	        // {
	        // 	break;
	        // }

    		if (readVarName == true)	//if we haven't read any labels and havent reached the end of the program, we add currently read characters to the label variable
    		{
    			varName += line[i];	//add the current character to the label string
    			if (varName == "END:")	//Same as above for the end.
    			{
    				ended = true;
    			}
        	}

	        //cout << varName << endl;
	        //at this point, we should have a varName to save to the varNames vector
		}
		varNames.push_back(varName);	//save the varName to the vector
	}
	file.close();
	return true;
}

void display()
{
	for (int i = 0; i < (int)symbolTable.size(); i++)	//For each Symbol in the symbol table, print its lavel and address
	{
		cout << "Label: " << symbolTable.at(i).label << endl;
		cout << "Address: " << symbolTable.at(i).address << endl;
	}
	cout << "Lines: " << lines << endl;
	for (int i = 0; i < (int)firstBuffer.size(); i++)	//For each Symbol in the symbol table, print its lavel and address
	{
		cout << "Set: " << firstBuffer.at(i) << endl;
	}
	for (int i = 0; i < (int)varNames.size(); i++)
	{
		cout << "TEST" << endl;
		cout << "Var name: " << varNames.at(i) << endl;
	}
}
