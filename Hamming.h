#ifndef Hamming_H
#define Hamming_H

#include "Hashtable.h"

class Hamming
{
	private:
		int L;
		H_HashTable *hashtable;	
	public:
		Hamming(int, int);
		~Hamming();
		
		void Random_Simple(int);
		
		void Display();
};

#endif
