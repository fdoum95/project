#ifndef Hamming_H
#define Hamming_H

#include <fstream>
#include <iostream>
#include "Hashtable.h"

class Hamming
{
	private:
		int L;
		int bsize;
		double R;
		double c;
		HashTable<char, int> *hashtable;	
	public:
		Hamming(int, int, int);
		~Hamming();
		
		int bsize_return();
		int get_L();
		
		void set_R(double);
		void set_c(double);
		
		void Hamming_Reader(char *, int, int);
		
		void printList();
		
		void Hamming_LSH(char *, std::ofstream& output, int, char);
};

#endif
