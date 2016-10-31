#ifndef Node_H
#define Node_H

template <class T>
class Node
{
	private:
		int number;
		T *data;
		int size;
		Node *next;
	public:
		Node(T*, int, int);
		~Node();
		
		T* Data_Return();
		
		void set_next(Node<T> *);
		Node<T>* get_next();
		int get_size();
		T* get_data();
		int get_Number();
		
		void printNode();
};	

#endif
