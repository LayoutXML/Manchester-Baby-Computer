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

struct Symbol 
{
	string label;
	string address;
};

struct Operand {
	string name;
	string value;
	int lineNum;
};

vector <Symbol> symbolTable;	//Symbol table is used for main program

vector <Operand> operandTable;	//Operand table is used for variables

vector <string> outputBuffer;	//Output buffer is used to print binary machine code lines to file
vector <string> firstBuffer;	//Buffer to print commands
vector <string> temp; 			//Temp buffer to hold variables before code starts


string inputFile = "Multiply2No.txt";	//Assembly language text file is read in
string outputFile = "output.txt";		//Machine code (binary) text file is produced

bool started = false;					//Booleans which are used to flag different states of the code
bool ended = false;						
bool finishFirstPass = false;			
int lines = 0; 							//Keeps track of how many lines there are in the machine code
int varsBeforeStart = 0;				//Keeps track of how many variables there are before the START keyword

bool firstPass();						//Function which goes through inputFile once and saves commands to the SymbolTable
bool secondPass();						//Function which goes through the inputFile again, saving variables to the variableTable. Similar to firstPass()
void display();							//Function which displays desired content to the terminal
bool instructionSet(); 					//Initialises the Instruction Set we're using in this Assembly Language Program
string convertToBinary(string num);		//Converts the input string to binary ++++
string convertToBinary2(int number);	//Converts the input int to binary ++++
string addZeros(int amount, int chars);	//Adds the correct amount of zeros to the input int so the machine code is formatted correctly
void convertToMachineCode();			//Converts everything to machine code by saving it to the output buffer
void printToFile();						//Prints the contents of the output buffer to the outputFile

int main()
{
	if (firstPass()) 
	{
		display();
		instructionSet();
		secondPass();
		display();
		convertToMachineCode();
		display();
		printToFile();
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
	bool readLabel = false;		//boolean which tells us a label has been read
	bool readAddress = false;	//boolean that tells us an address has been read

    if (!file) //if we can't open the file, print an error message
  	{
        cout << "Error, cannot open a file." << endl;
        return false;
    }
    else 	//If we can open the file, we need to read commands
    {
        while (getline(file, line))	//while a line can be read, we go on to the next line
        {
        	label = "";	//initializing local variables
        	address = "";
        	readLabel = false;
        	readAddress = false;

        	for (int i = 0; i < (int)line.length(); i++)	//reach each character in the current line
        	{
	        	if (line[i] == ';')	//break when we hit an endline character
	        		break;
				if (line[i] == ' ' || line[i] == '\t')	//continue when we read a space/tab unless we've started saving information, in which case change bool value
	        	{ 
	        		if (label != "")	
	        			readLabel = true;
	        		if (address != "")	
	        			readAddress = true;
	        		else 
	        			continue;
	        	}
	        	else	//if we read a character that is anything but a space or an end-of-line character, we need to save it somewhere 
	        	{ 
	        		if (!readLabel)	//if we haven't read any labels, we add currently read characters to the label variable
	        		{
	        			label += line[i];	
	        			if (label == "START:")	//if we read the 'START:' flag, reset the started bool and continue
	        			{
	        				started = true;
	        				label = "";
	        				continue;
	        			}
	        			else if (label == "END:")	//same as above for the end.
	        			{
	        				ended = true;
	        				label = "";
	        				continue;
	        			}
	        		}
	        		else if (!readAddress)	// if we haven't read an address, we add currently read characters to the address variable
	        			address += line[i];
	        	}
	        }
	       	if (label != "" && started == true)	//If we're in the functional section of the program, we save current symbols to the table
	       	{
	   			Symbol newSymbol;	
				newSymbol.label = label;
				newSymbol.address = address;
				symbolTable.push_back(newSymbol);
	       		lines++;	//increment the lines variable as this is relevant to the output file
	       	}
	       	if (started == true && ended == true)	//Once we hit the end, break
	       		break;
	    }
    }
    file.close();	//Close the file once we're finished with it
	return true;
}

bool secondPass()
{
	ended = false;	//Initialise bools
	started = false;
	
	ifstream file (inputFile);	//Open input file (no need to check as we've already checked in firstPass())
	
	bool readName;	//Declaring local variables: bool to tell us if a name has been read
   	bool readValue;	//Tells us if a value has been read
	string name;	//The name of a variable being read
	string value;	//The value of a variable being read
	string line;	//Each line of the file being read
	
	while(getline(file, line))
	{
		name = "";	//Initialise variables
		value = "";
    	readName = false;
    	readValue = false;

    	for (int i = 0; i < (int)line.length(); i++)
    	{
        	if (line[i] == ';')	
        		break;
        	if (line[i] == ' ' || line[i] == '\t')	
        	{
        		if (name != "" )	
        			readName = true;
        		if (value != "")	
        			readValue = true ;
        		else 
        			continue;	
        	}
        	else
        	{ 
        		if (readName == false)	
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
        				name = "";
        				break;
        			}
        		}
        		else if (readValue == false)	
        		{
        			if(line[i] != ':')		//For the second pass, we need to ignore ':' so we can save variable names
        				value += line[i];
        			if (value == "VAR")		//We reset value to "" if we read VAR as this simply means variable
        			{
        				value = "";
        				continue;
        			}
        		}
	        }
	    }
       	if (name != "" && value != "" && ended == true )	//We increment lines and push the read variable to operandTable
       	{
       		lines++;
       		string convertedValue = convertToBinary(value);
       		temp.push_back(convertedValue);
			Operand newOperand;	
			newOperand.name = name;
			newOperand.value = convertedValue;
			newOperand.lineNum = lines;
			operandTable.push_back(newOperand);
		}

		if (started == false && ended == false && name == "VAR")	//We increment the counter for first vars if we read a variable at the start with no name
		{
			varsBeforeStart++;
			string convertedValue = convertToBinary(value);
			temp.push_back(convertedValue);
		}
		if (started == false && ended == false && name != "VAR" && value != "")	//We save a variable to the operandTable and increment the counter
       	{
       		string convertedValue = convertToBinary(value);
       		temp.push_back(convertedValue);
			Operand newOperand;	
			newOperand.name = name;
			newOperand.value = convertedValue;
			newOperand.lineNum = varsBeforeStart;
			operandTable.push_back(newOperand);
			varsBeforeStart++;
		}
	}
	file.close();
	return true;
}

bool instructionSet()
{
	for (int i = 0; i < (int)symbolTable.size(); i++)	//Compares saved symbols with the instruction set below and pishes onto the first buffer if identical
	{
		if (symbolTable.at(i).label == "JMP")
			firstBuffer.push_back("000");
		else if (symbolTable.at(i).label == "JRP")
			firstBuffer.push_back("100");
		else if (symbolTable.at(i).label == "LDN")
			firstBuffer.push_back("010");
		else if (symbolTable.at(i).label == "STO")
			firstBuffer.push_back("110");	
		else if (symbolTable.at(i).label == "SUB")
			firstBuffer.push_back("001");
		else if (symbolTable.at(i).label == "SUB")
			firstBuffer.push_back("101");
		else if (symbolTable.at(i).label == "CMP")
			firstBuffer.push_back("011");	
		else if (symbolTable.at(i).label == "STP")
			firstBuffer.push_back("111");	
		else if (symbolTable.at(i).label == "LDP")
			firstBuffer.push_back("0001");	
		else if (symbolTable.at(i).label == "MTP")
			firstBuffer.push_back("1001");	
		else 
		{
			cout << "Error, could not find an instruction: " << symbolTable.at(i).label << "." << endl;
			return false;
		}
	}
	return true;
}

void convertToMachineCode()
{
	int count = 0;	//Keeps track of each line of code in the output file
	for(int i = 0; i < varsBeforeStart; i++)	//For each variable before START: we push the variable to the output buffer
		outputBuffer.push_back(temp.at(i));
	for (int i = 0; i < (int)symbolTable.size(); i++)	//For each instruction in the Symbol Table, we need to add it to the output buffer
	{
		string stringMaster = "";	//Initialize a master string to ""
		if (symbolTable.at(i).address == "")	//If there is no address given, we always add 13 0s so that the machine code adds up to 32 bits
		{
			for (int a = 0; a < 13; a++)
				stringMaster += "0";
		}
		else 	//If an address is given, we need to convert that to binary
		{
			for(int j = 0; j < (int)operandTable.size(); j++) //checks the address matches a valid variable and adds it to stringMaster
			{
				if (symbolTable.at(i).address == operandTable.at(j).name)
					stringMaster = stringMaster + convertToBinary2(operandTable.at(j).lineNum);
			}
		}
		int chars = (int)firstBuffer.at(count).size();	//We get the size in bits of the instruction
		stringMaster = stringMaster + firstBuffer.at(count) + addZeros(19,chars);	//Then add it to the output buffer with the right amount of 0s
		outputBuffer.push_back(stringMaster);
		count++;
	}
	for (int i = varsBeforeStart; i < (int)temp.size(); i++)	//Now just push each final variable at the end of the code to the output buffer
		outputBuffer.push_back(temp.at(i));
}

string addZeros(int amount, int chars)
{
	string returnString = "";
	for(int i = 0 + chars; i < amount; i++)
		returnString += "0";
	return returnString;
}

string convertToBinary2(int number)	//converts the input number to 13 bit binary through a looping division by 2
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

string convertToBinary(string num)	//converts the input number to 32 bit binary through a looping division by 2
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

	/*for (int i = 0; i < (int)temp.size(); i++)
	{
		cout << temp.at(i) << endl;
	}*/
}

void printToFile()
{
	ofstream out (outputFile);
	for (int i = 0; i < (int)outputBuffer.size(); i++)
		out << outputBuffer.at(i) << endl;
	out.close();
}
