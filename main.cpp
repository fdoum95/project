#include <fstream>
#include <sstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "System.h"


using namespace std;

void Menu(System *);

int main(int argc, char *argv[])
{
	
	cout << "START" << endl;
	char algorithm = 'H';
	int k = 4;
	int L = 5;
	string line;
	ifstream inFile;
	ifstream qFile;
	ofstream outFile;
	
	if (argc < 6)
	{
		cout << "Too few arguments" << endl;
		return -1;
	}
	
	for(int i=0; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0)
		{
			inFile.open(argv[i+1]);
		}
		else if (strcmp(argv[i], "-q") == 0)
		{
			qFile.open(argv[i+1]);
		}
		else if (strcmp(argv[i], "-k") == 0)
		{
			k = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-L") == 0)
		{
			L = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			outFile.open(argv[i+1]);
		}
	}

	char metric = 'H';
	System *_sys = new System(metric, k, L);
	
	Menu(_sys);
	
	if (inFile.is_open())
	{
		while(getline(inFile, line))
		{
			//cout << line << endl;
		}
	}
	
	inFile.close();
	qFile.close();
	outFile.close();	
	return 0;
}

void Menu(System *system)
{
	string option;
	cout << "You have entered the Menu Control" << endl;
	while(1)
	{
		cout << "Give an option" << endl;
		cin >> option;
		cout << "Do you want to terminate the program (Y/N)" << endl;
		cin >> option;
		if (option == "Y")
		{
			cout << "Exit Menu control" << endl;
			break;
		}
	}
}
