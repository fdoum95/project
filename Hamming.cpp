#include "Hamming.h"
#include "hashtable.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

Hamming::Hamming(int k, int l, int bit_size)
{
	cout << "A Hamming class was created." << endl;
	bsize = bit_size;
	L = l;
	hashtable = new H_HashTable[l];
	for (int i=0; i <= l - 1; i++)
	{
		hashtable[i].Hashtable_Init(k);
	}
}

Hamming::~Hamming()
{
	delete hashtable;
}

void Hamming::Display(){
	for (int i=0; i <= L - 1; i++)
	{
		hashtable[i].Display();
	}
}

void Hamming::Random_Simple(int size){
	for (int i=0; i <= L - 1; i++)
	{
		hashtable[i].Random_Simple(size);
	}
}

int Hamming::bsize_return(){
	return bsize;
}


template class List<int>;
template class List<char>;
