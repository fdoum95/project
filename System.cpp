#include "System.h"
#include "Hamming.h" 
#include <iostream>
#include <string.h>

using namespace std;

System::System(char m, int k, int L)
{
	metric = m;
	if (metric == 'H')
	{
		cout << "A new system was created with hamming metric." << endl;
		hamming = new Hamming(k, L);
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
	if (metric == 'H')
	{
		//new node
		//Insert list
	}
	else if (metric == 'E')
	{
		
	}
	else if (metric = 'C')
	{
		
	}
}


