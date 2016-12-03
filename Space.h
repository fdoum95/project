#ifndef Space_H
#define Space_H

#include <fstream>
#include <sstream>
#include <string.h>
#include <iostream>
#include "ClusterHamming.h"
#include "ClusterEuclidean.h"
#include "ClusterCosine.h"
#include "ClusterDistanceMatrix.h"

class Space
{
	public:
		char metric_space;
		int item_dimensions;
		int cluster_number;		//initialised by constructor
		
		int hash_functions;
		int hash_tables;
		int clarans_franction;
		int clarans_iterations;
		
		int **array;
		
		ClusterHamming *c_hamming;
		ClusterEuclidean *c_euclidean;
		ClusterCosine *c_cosine;
		ClusterDistanceMatrix *c_dmatrix;
	public:
		Space();
		~Space();
		
		void Space_Menu(std::ifstream& infile1, std::ifstream& infile2,std::ifstream& cfile, std::ofstream& outfile);
		void Space_Metric_Dims(std::ifstream& infile);
		void Space_Cluster_Conf(std::ifstream& cfile);
		
		char Metric_Space_Selection(char *line);
		void Metric_Initialization();
}; 

#endif
