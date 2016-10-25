#include <iostream>
#include "Node.h"

using namespace std;

template <class T>
Node<T>::Node()
{
	cout << "A Node was created." << endl;
}

template <class T>
Node<T>::~Node()
{
	delete data;
}

template class Node<int>;
template class Node<char>;
