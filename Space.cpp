#include <fstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "Space.h"

using namespace std;

Space::Space()
{
	cout << "A Space was constructed." << endl;
	cluster_number = 5;	//default
	c_hamming = NULL;
	c_euclidean = NULL;
	c_cosine = NULL;
	c_dmatrix = NULL;
}

Space::~Space()
{
	if (metric_space == 'h' && c_hamming != NULL)
		delete c_hamming;
	else if (metric_space == 'e' && c_euclidean != NULL)
		delete c_euclidean;
	else if (metric_space == 'c' && c_cosine != NULL)
		delete c_cosine;
	else if (metric_space == 'd' && c_dmatrix != NULL)
		delete c_dmatrix;
	cout << "A Space was destructed." << endl;
}

void Space::Space_Menu(ifstream& infile1, ifstream& infile2,ifstream& cfile, ofstream& outfile)
{
	cout << "Choosing Metric..." << endl;
	Space_Metric_Dims(infile1);		//Find the Metric , Estimate the dimension of data for each item 			
	
	Space_Cluster_Conf(cfile);
	
//	metric_space = 'c';			//for cosine using an euclidean file (the only change is in LSH)
	
	Metric_Initialization();		//Construct all classes that we need for the chosen metric
	//cout << "metric : " << metric_space << endl;
	//cout << "item_dimensions : " << item_dimensions << endl;
	//cout << "cluster_number : " << cluster_number << endl;
		
	if (metric_space == 'h')
		c_hamming->ClusterHamming_Menu(infile2, cfile, outfile);
	else if (metric_space == 'e')
		c_euclidean->ClusterEuclidean_Menu(infile2, cfile, outfile);
	else if (metric_space == 'c')
		c_cosine->ClusterCosine_Menu(infile2, cfile, outfile);
	else if (metric_space == 'd')
		c_dmatrix->ClusterDistanceMatrix_Menu(infile2, cfile, outfile, array);
		
}

void Space::Space_Metric_Dims(ifstream& infile)			//Estimate metric and dimensions for items
{
	char metric;
	string line;
	int line_count = 0, bit_size=0, tabs = 0, commas = 0;
	if (infile.is_open())
	{
		getline(infile, line);
		metric = Metric_Space_Selection(&line[0]);
		if (metric == 'h' || metric == 'H')						//Hamming
		{
			cout << "Metric is Hamming" << endl;
			//Estimate the number of bits for Hamming
			getline(infile, line);
			while(line[line_count] != '\t')
				line_count++;
			line_count++;
			bit_size = 0;
			while(line[line_count] != '\0')
			{
				line_count++;
				bit_size++;
			}
			metric_space = 'h';
			item_dimensions = bit_size;
		}
		else if(metric == 'v' || metric == 'e' || metric == 'c' || metric == 'V' || metric == 'E' || metric == 'C')				//vector (euclidean , cosine)
		{
			getline(infile, line);
			metric = Metric_Space_Selection(&line[0]);
			if (metric == 'c' || metric == 'C')
			{
				cout << "Metric is Cosine" << endl;
				metric_space = 'c';
			}
			else{
				cout << "Metric is Euclidean" << endl;
				metric_space = 'e';
			}
			//Estimate the number of dimensions for Cosine or Euclidean
			getline(infile, line);
			while(line[line_count] != '\0')
			{
				if (line[line_count] == '\t')				//number of tabs is equal to number of dimensions
					tabs++;
				line_count++;
			}
			item_dimensions = tabs;
		}
		else												// DistanceMatrix
		{
			cout << "Metric is Distance Matrix" << endl;
			metric_space = 'd';
			getline(infile, line);
			while(line[line_count] != '\0')
			{
				if (line[line_count] == ',')
					commas++;
				line_count++;				
			}
			commas++;
			item_dimensions = commas;
			array = new int*[item_dimensions];
			for (int i = 0; i <= item_dimensions - 1; i++)
				array[i] = new int[item_dimensions];
			
			int in, tn, jn=0;
			char str[3];
			while(getline(infile, line))			//save matrix 
			{
				line_count = 1;
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
}

char Space::Metric_Space_Selection(char *line)
{
	int line_count = 0;
	while(line[line_count] != ' ')
		line_count++;	
	line_count++;
	return line[line_count];		
}


void Space::Space_Cluster_Conf(ifstream& cfile)
{
	char *Num = new char[10];
	string line;
	int line_count = 0, count = 0;
	if (cfile.is_open())
	{
		getline(cfile, line);
		while(line[line_count] != ':')
			line_count++;
		line_count += 2;
		count = 0;
		while(line[line_count] != '\0')
		{
			Num[count] = line[line_count];
			count++;
			line_count++;
		}
		Num[count] = '\0';
		cluster_number = atoi(Num);		//cluster number
		
		
		line_count = 0;
		getline(cfile, line);
		while(line[line_count] != ':')
			line_count++;
		line_count += 2;
		count = 0;
		while(line[line_count] != '\0')
		{
			Num[count] = line[line_count];
			count++;
			line_count++;
		}
		Num[count] = '\0';
		hash_functions = atoi(Num);		//hash functions number
		
		
		line_count = 0;
		getline(cfile, line);
		while(line[line_count] != ':')
			line_count++;
		line_count += 2;
		count = 0;
		while(line[line_count] != '\0')
		{
			Num[count] = line[line_count];
			count++;
			line_count++;
		}
		Num[count] = '\0';
		hash_tables = atoi(Num);		//hash tables number		
		
		
		line_count = 0;
		getline(cfile, line);
		while(line[line_count] != ':')
			line_count++;
		line_count += 2;
		count = 0;
		while(line[line_count] != '\0')
		{
			Num[count] = line[line_count];
			count++;
			line_count++;
		}
		Num[count] = '\0';
		clarans_franction = atoi(Num);		//clarans fraction (the least)	
		
		
		line_count = 0;
		getline(cfile, line);
		while(line[line_count] != ':')
			line_count++;
		line_count += 2;
		count = 0;
		while(line[line_count] != '\0')
		{
			Num[count] = line[line_count];
			count++;
			line_count++;
		}
		Num[count] = '\0';
		clarans_iterations = atoi(Num);		//clarans iterations
	}
	cout << "cluster number is " << cluster_number << endl;
	cout << "hash functions number is " << hash_functions << endl;
	cout << "hash tables number is " << hash_tables << endl;
	cout << "clarans franction is " << clarans_franction << endl;
	cout << "clarans iterations number is " << clarans_iterations << endl;
}


void Space::Metric_Initialization()
{
	if (metric_space == 'h')
		c_hamming = new ClusterHamming(cluster_number, item_dimensions, hash_functions, hash_tables, clarans_franction, clarans_iterations);
	else if (metric_space == 'e')
		c_euclidean = new ClusterEuclidean(cluster_number, item_dimensions, hash_functions, hash_tables, clarans_franction, clarans_iterations);
	else if (metric_space == 'c')
		c_cosine = new ClusterCosine(cluster_number, item_dimensions, hash_functions, hash_tables, clarans_franction, clarans_iterations);
	else if (metric_space == 'd')
		c_dmatrix = new ClusterDistanceMatrix(cluster_number, item_dimensions, hash_functions, hash_tables, clarans_franction, clarans_iterations);	
}


