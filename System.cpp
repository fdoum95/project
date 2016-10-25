#include "System.h"
#include "Hamming.h" 
#include <iostream>

using namespace std;

System::System(char metric, int k, int L)
{
	if (metric = 'H')
	{
		hamming = new Hamming(k, L);
		cout << "A new system was created with hamming metric." << endl;
	}
}

System::~System()
{
	delete hamming;
}
