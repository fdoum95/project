#ifndef Cosine_H
#define Cosine_H

#include "Hashtable.h"
#include <iostream>
#include <fstream>

class Cosine
{
	private:
		int L;
		int dims;
		double R;
		double c;
		HashTable<double, double> *hashtable; 
	public:
		Cosine(int, int, int);
		~Cosine();
		
		void Cosine_Reader(char *, int, int);
	
		int get_L();
		
		void set_R(double);
		void set_c(double);

		void printList();

		void Cosine_LSH(char *, std::ofstream& output, int, char);
};

#endif
