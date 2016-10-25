#include "hashtable.h"
#include <cstdlib>
#include <iostream>
#include <math.h>

#include "bucket.h"

using namespace std;
HashTable::HashTable(int k, int L)
{
	size = pow(2, k);
	bucket = new Bucket*[L];
	for(int i=0; i<=L-1; i++)
	{
		bucket[i] = new Bucket[size];
		for(int j=0; j<=size-1; j++)
		{
			bucket[i][j].Hfunctions_Init(k, i, j);
		}
	}
	cout << "A hastable was created" << endl;
}

HashTable::~HashTable()
{
}

