#ifndef System_H
#define System_H

#include "Hamming.h" 

class System
{
	private:
		Hamming *hamming;
		
		
	public:
		System(char , int, int);
		~System();
};

#endif
