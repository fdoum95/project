#ifndef DistanceMatrix_H
#define DistanceMatrix_H

#include <fstream>
#include <iostream>
#include <math.h>

#include "Hashtable.h"

class DistanceMatrix
{
	private:
		int L;
		int nItems;
		double R;
		double c;
		int** table;
		HashTable<int, int> *hashtable;	
	public:
		DistanceMatrix(int, int, int, int**);
		~DistanceMatrix();
		
		int get_nitems();
		
		int get_L();
		
		void set_R(double);
		void set_c(double);
		
		void DistanceMatrix_Reader(int, int, int);
		
		List<int> *LSH_Cluster_DistanceMatrix(Node<int> *centroid);
		
		void printList();
		
		void DistanceMatrix_LSH(char *, std::ofstream& output, int, char);
		
		void set_Median(Node<int> *, int);
};

#endif
