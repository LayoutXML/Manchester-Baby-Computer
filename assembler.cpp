#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

struct Symbol {
	string label;
	string address;
};

vector <Symbol> symbolTable;
vector <int> outputBuffer; //binary

//variables
string inputFile = "input.txt";
string outputFile = "output.txt";
bool started = false;
bool ended = false;
bool finishFirstPass = false;

//functions
bool processFile();
void display();

int main() {
	
	if (processFile()) 
	{
		display();
	} 
	else 
	{
		return 1;
	}
	return 0;
}

bool processFile() 
{
	string line;
	string label = "";
	string address = "";
	bool readLabel = false;
	bool readAddress = false;

    ifstream file (inputFile);
    if (!file)
  	{
        cout << "Error, cannot open a file." << endl;
        return false;
    }
    else
    {
        while (getline(file, line))
        {
        	label = "";
        	address = "";
        	readLabel = false;
        	readAddress = false;

        	for (int i = 0; i < (int)line.length(); i++)
        	{
	        	if (line[i] == ';')
	        	{
	        		break;
	        	}
	        	if (line[i] == ' ')
	        	{
	        		if (label != "")
	        		{
	        			readLabel = true;
	        		}
	        		if (address != "")
	        		{
	        			readAddress = true;
	        		}
	        		else 
	        		{
	        			continue;
	        		}
	        	}
	        	else
	        	{ 
	        		if (readLabel == false && ended == false)
	        		{
	        			label += line[i];

	        			if (label == "START:")
	        			{
	        				started = true;
	        				label = "";
	        				continue;
	        			}
	        			else if (label == "END:")
	        			{
	        				ended = true;
	        				label = "";
	        			}
	        		}
	        		else if (readAddress == false && ended == false)
	        		{
	        			address += line[i];
	        		}
	        	}
	        }

	       	if (label != "" && address != "" && started == true)
	       	{
	       		if ( ended != true)
	       		{
					Symbol newSymbol;
					newSymbol.label = label;
					newSymbol.address = address;
					symbolTable.push_back(newSymbol);
	       		}
	       		else
	       		{
	       			address = "";
	       			Symbol newSymbol;
					newSymbol.label = label;
					newSymbol.address = address;
					symbolTable.push_back(newSymbol);
	       		}
				//cout << "LABEL - " << label << endl;
	       		//cout << "ADDRESS - " << address << endl;
	       	}

	       	if (started == true && ended == true)
	       	{
	       		break;
	       	}

	    }
    }
    file.close();
	return true;
}

void display()
{
	for (int i = 0; i < (int)symbolTable.size(); i++)
	{
		cout << "Label: " << symbolTable.at(i).label << endl;
		cout << "Address: " << symbolTable.at(i).address << endl;
	}
}