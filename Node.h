#ifndef Node_H
#define Node_H

template <class T>
class Node
{
	private:
		T *data;
		Node *next;
	public:
		Node();
		~Node();
};

#endif
