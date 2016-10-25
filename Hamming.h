#ifndef Hamming_H
#define Hamming_H

#include "Hashtable.h"

class Hamming
{
	private:
		int bsize;
		int L;
		H_HashTable *hashtable;	
	public:
		Hamming(int, int, int);
		~Hamming();
		
		void Random_Simple(int);
		
		void Display();
		
		int bsize_return();
};

#endif
