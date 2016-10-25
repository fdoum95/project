#include "Hamming.h"
#include "List.h"
#include <iostream>
#include <math.h>

using namespace std;


//Hashtable

H_HashTable::H_HashTable()
{
}

H_HashTable::~H_HashTable()
{
	delete Hfunctions;
	delete list;
}

H_HashTable::Hashtable_Init(int k)
{
	cout << "A Hashtable was created." << endl;
	size = pow(2, k);
	Hfunctions = new int[k];
	list = new List<char>[size];
}



//Hamming

Hamming::Hamming(int k, int l)
{
	cout << "A Hamming class was created." << endl;
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

template class List<int>;
template class List<char>;
