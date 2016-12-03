#include <fstream>
#include <sstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>  //rand, RAND_MAX
#include <ctime>
#include <iomanip>

#include "Space.h"

using namespace std;

int main(int argc, char *argv[])
{	
	Space *S;
	ifstream inFile1;
	ifstream inFile2;
	ifstream cFile;
	ofstream outFile;
	
    srand(time(NULL));
	
	if (argc < 3)
	{
		cout << "Too few arguments" << endl;
		return -1;
	}
	
	for(int i=0; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0)
		{
			inFile1.open(argv[i+1]);
			inFile2.open(argv[i+1]);
		}
		else if (strcmp(argv[i], "-c") == 0)
		{
			cFile.open(argv[i+1]);
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			outFile.open(argv[i+1]);
		}
	}

	cout << "Project 2 is starting." << endl;
	S = new Space();
	S->Space_Menu(inFile1, inFile2, cFile, outFile);
	
	delete S;
	return 0;
}

