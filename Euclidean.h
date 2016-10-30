#ifndef Euclidean_H
#define Euclidean_H

#include "Hashtable.h"
#include <iostream>
#include <fstream>

class Euclidean
{
	private:
		int L;
		int dims;
		double R;
		double c;
		int **random;
		HashTable<double,double> *hashtable; 
	public:
		Euclidean(int, int, int, int, int**);
		~Euclidean();
		
		void Euclidean_Reader(char *, int, int);
		int get_L();
		
		void set_R(double);
		void set_c(double);

		void printList();
		
		void Euclidean_LSH(char *, std::ofstream& output, int);
};

#endif
