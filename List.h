#ifndef List_H
#define List_H

#include <fstream>
#include <iostream>
#include "Node.h"

template <class T>
class List
{
	private:
		int list_size;
		Node<T> *start;
	public:
		List();
		~List();
		
		bool is_empty();
		void Insert_Node(Node<T> *);
		
		void printList();
		
		Node<T> *get_start();
		void set_start(Node<T> *);
		
		void Insert_List(List<T> *, int);
		
		bool Check(Node<T>* );
		
		int Hamming_Distance(Node<T>* , double, double, std::ofstream& output, int tag);
		double Euclidean_Distance(Node<T>* , double, double, std::ofstream& output, int tag);
		double Cosine_Distance(Node<T>* , double, double, std::ofstream& output, int tag);
		int DistanceMatrix_Distance(Node<T>* , double, double, std::ofstream& output, int tag);
};

#endif
