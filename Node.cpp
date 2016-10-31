#include <iostream>
#include "Node.h"
#include <iomanip>

using namespace std;

template <class T>
Node<T>::Node(T* Dt, int s, int N)
{
	//cout << "A Node was created." << endl;
	number = N;
	data = Dt;
	size = s; //xreiazetai mono sthn cout opote mallon 8a afaire8ei
	next = NULL;
}

template <class T>
Node<T>::~Node()
{
	delete data;
}

template <class T>
T* Node<T>::Data_Return()
{
	return data;
}


template <class T>
void Node<T>::set_next(Node<T> *node)
{
	next = node;
}

template <class T>
Node<T>* Node<T>::get_next()
{
	return next;
}

template <class T>
int Node<T>::get_size()
{
	return size;
}

template <class T>
T* Node<T>::get_data()
{
	return data;
}

template <class T>
void Node<T>::printNode()
{
//	cout << data << "     ";
	cout << number << "   ";
//	for(int i=0; i <= size-1; i++)
//		cout << setprecision(12) << data[i] << "   ";
//	cout << endl;
}

template <class T>
int Node<T>::get_Number()
{
	return number;
}


template class Node<int>;
template class Node<char>;
template class Node<double>;
