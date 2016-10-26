#ifndef System_H
#define System_H

#include "Hamming.h"
#include <string.h> 

class System
{
	private:
		char metric;
		Hamming *hamming;
		
		
	public:
		System(char , int, int);
		~System();
		
		void Menu();
		
		void Random_Generator(char, int);
		
		void Display();
		
		void Reader(char *);
};

#endif
