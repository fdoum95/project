#ifndef Hashtable_H
#define Hashtable_H

#include "List.h"

class H_HashTable
{
	private:
		int k;
		int size;
		int *Hfunctions;
		List<char> *list;
	public:
		H_HashTable();
		~H_HashTable();
		
		void Hashtable_Init(int);
	
		void Random_Simple(int);
		
		void Display();
};

#endif
