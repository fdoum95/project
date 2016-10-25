#include <iostream>
#include "List.h"

using namespace std;

template <class T>
List<T>::List()
{
	cout << "A list was created." << endl;
	list_size = 0;
	start = NULL;
	end = NULL;
}

template <class T>
List<T>::~List()
{
	delete start;
	delete end;
}

template class List<int>;
template class List<char>;
