#ifndef List_H
#define List_H

#include "Node.h"

template <class T>
class List
{
	private:
		int list_size;
		Node<char> *start;
		Node<char> *end;
	public:
		List();
		~List();
};

#endif
