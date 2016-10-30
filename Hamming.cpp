#include <fstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <time.h>
#include "Hamming.h"
#include "Hashtable.h"
#include "List.h"
#include "Node.h"

using namespace std;

Hamming::Hamming(int k, int l, int bit_size)
{
	cout << "A Hamming class was created." << endl;
	bsize = bit_size;
	L = l;
	hashtable = new HashTable<char, int>[l];
	for (int i=0; i <= l - 1; i++)
	{
		hashtable[i].HashTable_Init(i, k, bit_size, pow(2, k), 'h');   //no 5th argument, so w = 0 by default
	}
}

Hamming::~Hamming()
{
	delete[] hashtable;
	cout << "Delete Hamming class" << endl;
}


int Hamming::bsize_return(){
	return bsize;
}

int Hamming::get_L(){
	return L;
}

void Hamming::set_R(double varR)
{
	R = varR;
}

void Hamming::set_c(double varc)
{
	c = varc;
}

void Hamming::Hamming_Reader(char *line, int Hash_index, int Number){
	int line_count = 0;
	char *bstring;
	bstring = new char[bsize_return()];
	Node<char>* node;
	
	while(line[line_count] != '\t')
		line_count++;
	line_count++;
	
	strcpy(bstring, &line[line_count]);  //copy bit string from line to bstring
	
	node = new Node<char>(&bstring[0], bsize_return(), Number);
	
	hashtable[Hash_index].Insert_Node(node);
}



void Hamming::Hamming_LSH(char *line, ofstream& output, int Number){  //Dont care to create one node for all hashtables
	double time_LSH, time_ALL;
	int line_count = 0, i=0;
	int dist;
	char *bstring;
	bstring = new char[bsize_return()];
	char name[4];
	Node<char>* node;
	
	List<char>* qlist;
	List<char>* final_list_LSH;
	List<char>* final_list_ALL;
	
	while(line[line_count] != '\t')
		line_count++;
	line_count++;
	
	strcpy(bstring, &line[line_count]);  //copy bit string from line to bstring
	
	node = new Node<char>(&bstring[0], bsize_return(), Number);
	
	qlist = new List<char>;
	final_list_LSH = new List<char>;
	final_list_ALL = new List<char>;
	
	output << "Query: Item";
	output << node->get_Number();
	output << '\n';
	
	//Search closer neighbor and if R > 0 find those with dist > R*c and the closest neighbor
	cout << "item_idS" << Number << '\t';
	const clock_t begin_time_LSH = clock();						//time	
	for(int i=0; i <= L -1; i++)
	{
		hashtable[i].HashTable_LSH(node, qlist);
		final_list_LSH->Insert_List(qlist);
	}

	dist = final_list_LSH->Hamming_Distance(node, R, c, output, 0);			//0 for LSH
	output << "distanceLSH: ";
	output << dist;
	output << '\n';
	
	time_LSH = (double)((clock() - begin_time_LSH ) /  (double)CLOCKS_PER_SEC);
	
	//Search closer neighbor and if R > 0 find tose with dist > R*c
	const clock_t begin_time_ALL = clock();							//time
	hashtable[0].HashTable_Search_All(node, final_list_ALL);

	dist = final_list_ALL->Hamming_Distance(node, R, c, output, 1);			//1 for ALL
	output << "distanceTrue: ";
	output << dist;
	output << '\n';
	time_ALL = (double)(( clock() - begin_time_ALL ) /  (double)CLOCKS_PER_SEC);			//time
	
	output << "tLSH: ";
	output <<  time_LSH;
	output << '\n';
	
	output << "tTrue: ";
	output << time_ALL;
	output << '\n';
	output << '\n';
	
	qlist->set_start(NULL);
	delete qlist;
	delete final_list_LSH;
	delete final_list_ALL;
}


void Hamming::printList(){
	for (int i=0; i <= L - 1; i++)
	{
		cout << "Hashtable " << i << endl;
		hashtable[i].printList();
	}
}


template class List<int>;
template class List<char>;
template class List<double>;

