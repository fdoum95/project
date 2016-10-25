#include <cstdlib>
#include <iostream>

#include "bucket.h"

using namespace std;

Bucket::Bucket()
{
	cout << "A Bucket was created" << endl;
}

Bucket::~Bucket()
{
	delete Hfunctions;
}

void Bucket::Hfunctions_Init(int k, int i , int j)
{
	Hfunctions = new int(k);
	cout << "Hfunctions was initiated for bucket " << j << " in g " << i << endl;
}
