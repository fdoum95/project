#include "Hashtable.h"
#include "List.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

H_HashTable::H_HashTable()
{
}

H_HashTable::~H_HashTable()
{
	delete Hfunctions;
	delete list;
}

void H_HashTable::Hashtable_Init(int nk)
{
	cout << "A Hashtable was created." << endl;
	k = nk;
	size = pow(2, k);
	Hfunctions = new int[k];
	list = new List<char>[size];
}


void H_HashTable::Random_Simple(int bitstr_len)
{
	for (int i=0; i <= k - 1; i++)
	{
		Hfunctions[i] = rand() % (bitstr_len - 1) + 1;
	}
}

void H_HashTable::Display()
{
	for (int i=0; i <= k - 1; i++)
	{
		cout << Hfunctions[i] << " ";
	}
	cout << endl;
}

