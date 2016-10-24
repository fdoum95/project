#include <iostream>
#include "List.h"

using namespace std;


//HNode

template <class T>
Node<T>::Node()
{
	cout << "A Node was created." << endl;
}

template <class T>
Node<T>::~Node()
{
	
}


//HList

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
