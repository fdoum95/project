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

void Hamming::Hamming_Reader(char *line, int Hash_index, int NumberTable){
	//Create Nodes for each item 
	int line_count = 0, it = 0;
	char *bstring;
	char NumItem[10];
	int ItemNumber = 0;
	bstring = new char[bsize_return()];
	Node<char>* node;
	
	while(line[line_count] != 'm')		//1' || line[line_count] != '2' || line[line_count] != '3' || line[line_count] != '4' || line[line_count] != '5' || line[line_count] != '6' || line[line_count] != '7' || line[line_count] != '8' || line[line_count] != '9')
		line_count++;
	line_count++;
	
	while(line[line_count] != '\t')
	{
		NumItem[it] = line[line_count];
		line_count++;
		it++;
	}
	NumItem[it] = '\0';
	line_count++;
	
	ItemNumber = atoi(NumItem);
	
	strcpy(bstring, &line[line_count]);  //copy bit string from line to bstring
	
	node = new Node<char>(&bstring[0], bsize_return(), ItemNumber, NumberTable);
	
	hashtable[Hash_index].Insert_Node(node);
}



void Hamming::Hamming_LSH(char *line, ofstream& output, int Number, char option){  //Dont care to create one node for all hashtables
	double time_LSH, time_ALL;
	int line_count = 0;
	int dist;
	char *bstring;
	bstring = new char[bsize_return()];
	char name[4];
	int NumberItems = 1000;
	Node<char>* node;
	
	List<char>* qlist;
	List<char>* final_list_LSH;
	List<char>* final_list_ALL;
	
	while(line[line_count] != '\t')
		line_count++;
	line_count++;
	
	strcpy(bstring, &line[line_count]);  //copy bit string from line to bstring
	
	qlist = new List<char>;
	final_list_LSH = new List<char>;
	final_list_ALL = new List<char>;
	
	output << "Query: Item";
	output << node->get_Number();
	output << '\n';

	if (option == 'y')
		NumberItems = 3*get_L();
		
	
	//Search closer neighbor and if R > 0 find those with dist > R*c and the closest neighbor
	cout << "item_idS" << Number << '\t';
	const clock_t begin_time_LSH = clock();						//time	
	for(int i=0; i <= L -1; i++)
	{
		hashtable[i].HashTable_LSH(node, qlist);
		final_list_LSH->Insert_List(qlist, NumberItems);
	}

	dist = final_list_LSH->Hamming_Distance(node, R, c, output, 0);			//0 for LSH
	output << "distanceLSH: ";
	output << dist;
	output << '\n';
	
	time_LSH = (double)((clock() - begin_time_LSH ) /  (double)CLOCKS_PER_SEC);
	
	//Search closer neighbor for ALL
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


List<char> *Hamming::LSH_Cluster_Hamming(Node<char> *centroid)			//return the final list that is belonged the centroid
{
	List<char>* qlist;
	List<char>* final_list_LSH;
	
	qlist = new List<char>;
	final_list_LSH = new List<char>;
	
	for(int i=0; i <= L -1; i++)
	{
		hashtable[i].HashTable_LSH(centroid, qlist);
		final_list_LSH->Insert_List(qlist);
	}
	
	return final_list_LSH;	
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

