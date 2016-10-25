#ifndef Hamming_H
#define Hamming_H

#include "List.h"

class H_HashTable
{
	private:
		int size;
		int *Hfunctions;
		List<char> *list;
	public:
		H_HashTable();
		~H_HashTable();
		
		Hashtable_Init(int);
};

class Hamming
{
	private:
		int L;
		H_HashTable *hashtable;	
	public:
		Hamming(int, int);
		~Hamming();
};

#endif
