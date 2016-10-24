#ifndef List_H
#define List_H

template <class T>
class Node
{
	private:
		T *bit_string;
		Node *next;
	public:
		Node();
		~Node();
};

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
