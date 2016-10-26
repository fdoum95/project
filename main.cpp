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
	char metric;
	int bit_size = 0;
	int line_count = 0, mspace_count = 0, count;
	char *metric_space;
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
	

	//choose metric
	if (inFile.is_open())
	{
		getline(inFile, line);
		while(line[line_count] != ' ')
			line_count++;
		
		line_count++;
		if (line[line_count] == 'h')
		{
			metric = 'H';
			cout << "Metric is Hamming" << endl;
		}
		else if(line[line_count] == 'v' || line[line_count] == 'e' || line[line_count] == 'c')
		{
			getline(inFile, line);
			while(line[line_count] != ' ')
				line_count++;
			line_count++;
			if (line[line_count] == 'c')
			{
				metric = 'H';
				cout << "Metric is Cosine" << endl;
			}
			else
			{
				metric = 'E';
				cout << "Metric is Euclidean" << endl;
			}
		}
		else
		{
			metric = 'M';
			cout << "Metric is Distance Matrix" << endl;
		}
	}
	
	
	//Creation a New System with all classes that structure includes
	System *_sys = new System(metric, k, L);
	
	//Estimate the number of bits for Hamming
	if (metric = 'H'){
		line_count = 0;
		if (inFile.is_open())
		{
			getline(inFile, line);
			while(line[line_count] != '\t')
				line_count++;
			line_count++;
			while(line[line_count] != '\0')
			{
				line_count++;
				bit_size++;
			}
		}
	}
	
	//Randomize
	_sys->Random_Generator('H', bit_size);
	
	//_sys->Menu();
	
	//Read Queries
	if (inFile.is_open())
	{
		int i = 0;
		do
		{
			cout << line << endl;
			i++;
			_sys->Reader(&line[0]);
		}while(getline(inFile, line) && i <=10);
	}
	
	inFile.close();
	qFile.close();
	outFile.close();	
	return 0;
}








//bo8ros

		/*
		getline(inFile, line);
		cout << line << endl;
		while(line[line_count] != ' ')
			line_count++;
		line_count++;
		while(line[count] != '\t'){
			count++;
		}
		metric_space = new char[count];
		while(line[line_count] != '\t'){
			metric_space[mspace_count] = line[line_count];
			mspace_count++;
			line_count++;
		}
		metric_space[mspace_count] = '\0';
		if (strcmp(metric_space, "hamming") == 0)
		{
			metric = 'H';
			cout << "Metric is Hamming" << endl;
		}
		else if (strcmp(metric_space, "vector") == 0)
		{
			delete metric_space;
			line_count = 0;
			mspace_count = 0;
			count = 0;
			getline(inFile, line);
			while(line[line_count] != ' ')
				line_count++;
			while(line[count] != '\t'){
				count++;
			}
			metric_space = new char[count];
			while(line[line_count] != '\t'){
				metric_space[mspace_count] = line[line_count];
				mspace_count++;
				line_count++;
			}
			metric_space[mspace_count] = '\0';
			if (strcmp(metric_space, "euclidean") == 0)
			{
				metric = 'E';
				cout << "Metric is Euclidean" << endl;
			}
			else if(strcmp(metric_space, "cosine") == 0)
			{
				metric = 'E';
				cout << "Metric is Cosine" << endl;
			}
		}*/

