#include "System.h"
#include "Hamming.h" 
#include <iostream>
#include <string.h>

using namespace std;

System::System(char m, int k, int L, int bit_size)
{
	metric = m;
	if (metric == 'H')
	{
		cout << "A new system was created with hamming metric." << endl;
		hamming = new Hamming(k, L, bit_size);
	}
}

System::~System()
{
	delete hamming;
}


void System::Menu()
{
	string option;
	cout << "You have entered the Menu Control" << endl;
	while(1)
	{
		Display();
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

void System::Random_Generator(char metric, int size)
{
	if (metric == 'H')
	{
		hamming->Random_Simple(size);
	}
	else if (metric == 'E')
	{
		
	}
	else if (metric = 'C')
	{
		
	}
}

void System::Display()
{
	hamming->Display();
}

void System::Reader(char *line)
{
	int line_count = 0;
	char *bstring;
	bstring = new char[hamming->bsize_return()];
	if (metric == 'H')
	{
		while(line[line_count] != '\t')
			line_count++;
		line_count++;
		
		strcpy(bstring, &line[line_count]);  //copy bit string from line to bstring
		cout << bstring << endl;
		
		//new node
		//Insert node at list
	}
	else if (metric == 'E')
	{
		
	}
	else if (metric = 'C')
	{
		
	}
}


