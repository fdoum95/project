#include <fstream>
#include <sstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>  //rand, RAND_MAX
#include <ctime>
#include <iomanip>

#include "Hamming.h"
#include "Euclidean.h"
#include "Cosine.h"
#include "DistanceMatrix.h"
#include "Random.h"

#define E_SIZE 16
#define MAXIMUM 1000
#define MAX_LSH 30

using namespace std;

char Metric_Space_Selection(char *);
double R_Calculation(char *);

int main(int argc, char *argv[])
{
	int tabs = 0;			//number of tabs is equal to the number of dimensions
	int commas = 0;
	int TableSize = 0;
	int bit_size = 0;
	int line_count = 0;
	int k = 4, L = 5;
	int max = 0;
	int** array;
	double R;
	double c = 1;
	char metric, option;
	string line;
	char command;
	
	Hamming *hamming;
	Euclidean *euclidean;
	Cosine *cosine;
	DistanceMatrix *dmatrix;
	
	ifstream inFile1;
	ifstream inFile2;
	ifstream qFile;
	ofstream outFile;
	
    srand(time(NULL));
	
	if (argc < 6)
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

	cout << "Program is starting." << endl;
	

do{
	//choose metric
	if (inFile1.is_open())
	{
		getline(inFile1, line);
		metric = Metric_Space_Selection(&line[0]);
		if (metric == 'h' || metric == 'H')						//Hamming
		{
			cout << "Metric is Hamming" << endl;
			metric = 'h';
			//Estimate the number of bits for Hamming
			line_count = 0;
			getline(inFile1, line);
			while(line[line_count] != '\t')
				line_count++;
			line_count++;
			bit_size = 0;
			while(line[line_count] != '\0')
			{
				line_count++;
				bit_size++;
			}
		}
		else if(metric == 'v' || metric == 'e' || metric == 'c' || metric == 'V' || metric == 'E' || metric == 'C')				//vector (euclidean , cosine)
		{
			getline(inFile1, line);
			metric = Metric_Space_Selection(&line[0]);
			if (metric == 'c' || metric == 'C')
			{
				cout << "Metric is Cosine" << endl;
				metric = 'c';
			}
			else{
				cout << "Metric is Euclidean" << endl;
				metric = 'e';
			}
			//Estimate the number of dimensions for Cosine or Euclidean
			line_count = 0;
			tabs = 0;
			getline(inFile1, line);
			while(line[line_count] != '\0')
			{
				if (line[line_count] == '\t')				//number of tabs is equal to number of dimensions
					tabs++;
				line_count++;
			}
			TableSize = 1;
			if (metric == 'e')								//only for euclidean
			{
				while(getline(inFile1, line) && TableSize <= MAXIMUM)
				{
					TableSize++;
				}
				array = new int*[L];				// for euclidean array = r (random)   , it s destructed in Euclidean
				for (int i = 0; i <= L - 1; i++)
					array[i] = new int[k];
			
				//Create an array with random values 
				for (int i = 0; i <= L - 1; i++)
					for (int j = 0; j <= k - 1; j++)
						array[i][j] = uniform_distribution(-32767, 32767);
			}
		}
		else												// DistanceMatrix
		{
			cout << "Metric is Distance Matrix" << endl;
			metric = 'd';
			line_count = 0;
			commas = 0;
			getline(inFile1, line);
			while(line[line_count] != '\0')
			{
				if (line[line_count] == ',')
					commas++;
				line_count++;				
			}
			commas++;
			cout << "Create an array of " << commas << " items" << endl;
			array = new int*[commas];
			for (int i = 0; i <= commas - 1; i++)
				array[i] = new int[commas];
			
			int in;
			int tn;
			int jn=0;
			char str[3];
			while(getline(inFile1, line))			//save matrix 
			{
				line_count = 0;
				in = 0;
				tn = 0;
				while(line[line_count] != '\0')
				{
					if (line[line_count] != '\t')
					{
						str[in] = line[line_count];
						in++;
						line_count++;
					}
					if (line[line_count] == '\t')
					{
						str[in] = '\0';
						array[jn][tn] = atof(str);
						line_count++;
						tn++;
						in = 0;
					}
				}
				str[in] = '\0';
				array[jn][tn] = atof(str); //last element				
				jn++;
			}
		}
	}
	
//Create new classes
//Read new items and add them to hashtables
	max = 1;
	if (metric == 'h')
	{
		hamming = new Hamming(k, L, bit_size);
		getline(inFile2, line);
		if (inFile2.is_open())
		{
			while(getline(inFile2, line) && max <= MAXIMUM)     //Start reading items and put them at lists
			{
				for(int i=0; i <= hamming->get_L() - 1; i++)    //for every HashTable  , that mean that we have L copies of a node
					hamming->Hamming_Reader(&line[0], i, max);
				max++;
			}
		}
	}
	else if (metric == 'e')
	{
		euclidean = new Euclidean(k, L, tabs, TableSize/E_SIZE, array);  //E_SIZE is defined
		getline(inFile2, line);
		getline(inFile2, line);
		if (inFile2.is_open())
		{
			while(getline(inFile2, line) && max <= MAXIMUM)     //Start reading items and put them at lists
			{
				for(int i=0; i <= euclidean->get_L() - 1; i++)    //for every HashTable  , that mean that we have L copies of a node
					euclidean->Euclidean_Reader(&line[0], i, max);
				max++;
			}
		}
	}
	else if (metric == 'c')
	{
		cosine = new Cosine(k, L, tabs);
		getline(inFile2, line);
		getline(inFile2, line);
		if (inFile2.is_open())
		{
			while(getline(inFile2, line) && max <= MAXIMUM)     //Start reading items and put them at lists
			{
				for(int i=0; i <= cosine->get_L() - 1; i++)    //for every HashTable  , that mean that we have L copies of a node
					cosine->Cosine_Reader(&line[0], i, max);
				max++;
			}
		}
	}
	else if (metric == 'd')
	{
		dmatrix = new DistanceMatrix(k, L, commas, array);
		getline(inFile2, line);
		getline(inFile2, line);
		if (inFile2.is_open())
		{
			while(getline(inFile2, line) && max <= MAXIMUM)     //Start reading items and put them at lists
			{
				for(int i=0; i <= dmatrix->get_L() - 1; i++)    //for every HashTable  , that mean that we have L copies of a node
					dmatrix->DistanceMatrix_Reader(i, max);
				max++;
			}
		}
	}
	cout << "Inserting nodes in lists was completed." << endl;
	inFile1.close();
	inFile2.close();
	
//Read queries from Query.txt-------radius
	line_count = 0;
	R = 0;
	cout << "Reading from Query File..." << endl;
	if (qFile.is_open())
	{
		//Ypologismos R
		getline(qFile, line);
		R = R_Calculation(&line[0]);
		cout << "R = " << R << endl;
	}
	
	//-----------------------------------LSH---------------------------------------------------
		
	cout << "Do you want to run for 3L neighbors?(y/n)" << endl;
	cin >> option;
	max = 1;
	if (metric == 'h')
	{
		hamming->set_R(R);
		hamming->set_c(c);
		while(getline(qFile, line) && max <= MAX_LSH)     //Start reading queries and put them at lists
		{
			hamming->Hamming_LSH(&line[0], outFile, max, option);
			max++;
		}
	}
	else if (metric == 'e')
	{
		euclidean->set_R(R);
		euclidean->set_c(c);
		while(getline(qFile, line) && max <= MAX_LSH)     //Start reading queries and put them at lists
		{
			euclidean->Euclidean_LSH(&line[0], outFile, max, option);
			max++;
		}
	}
	else if (metric == 'c')
	{
		cosine->set_R(R);
		cosine->set_c(c);
		while(getline(qFile, line) && max <= MAX_LSH)     //Start reading queries and put them at lists
		{
			cosine->Cosine_LSH(&line[0], outFile, max, option);
			max++;
		}
	}
	else if (metric == 'd')
	{
		dmatrix->set_R(R);
		dmatrix->set_c(c);
		while(getline(qFile, line) && max <= MAX_LSH)     //Start reading queries and put them at lists
		{
			dmatrix->DistanceMatrix_LSH(&line[0], outFile, max, option);
			max++;
		}
	}
	
	//free Memory
	if (metric == 'h')
	{
		delete hamming;
	}
	else if (metric == 'e')
	{
		delete euclidean;
		array = NULL;
	}
	else if (metric == 'c')
	{
		delete cosine;
	}
	else if (metric == 'd')
	{
		delete dmatrix;	
		array = NULL;	
	}
	
	
	qFile.close();
	
	do{
		cout << "Do you want to give new arguments (Y/N)?" << endl;
		cin >> command;		
	}while(command != 'Y' && command != 'N' && command != 'y' && command != 'n');
	
	if (command == 'Y' || command == 'y')
	{
		char *str_in = new char[30];
		cout << "Give the Input File with its path." << endl;
		cin >> str_in;
		inFile1.open(str_in);
		inFile2.open(str_in);
	
		cout << "Give the Query File with its path." << endl;
		cin >> str_in;
		qFile.open(str_in);
	
		cout << "Give new k value." << endl;
		cin >> str_in;
		k = atoi(str_in);
	
		cout << "Give new L value." << endl;	
		cin >> str_in;
		L = atoi(str_in);
		
		delete str_in;
	}
}while(command == 'Y' || command == 'y');
		
	cout << "Program exit..." << endl;
	outFile.close();
	return 0;
}

//-------------------------------------More Functions--------------------------------------------------

char Metric_Space_Selection(char *line)
{
	int line_count = 0;
	while(line[line_count] != ' ')
		line_count++;	
	line_count++;
	return line[line_count];		
}


double R_Calculation(char *line)
{
	int line_count = 0;
	char R[20];
	int i = 0;
	while(line[line_count] != ':')
		line_count++;
	line_count++;
	if (line[line_count] == ' ')
		line_count++;
	while(line[line_count] != '\0')
	{
		R[i] = line[line_count];
		line_count++;
		i++;
	}
	R[i] = '\0';
	return atof(R);
}

