#include "Euclidean.h"
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Node.h"
#include "List.h"
#include <iomanip>

using namespace std;

Euclidean::Euclidean(int var_k, int var_L, int dimensions, int tsize, int** array)
{
	cout << "An Euclidean class was constructed" << endl;
	L = var_L;
	dims = dimensions;
	random = array;
	hashtable = new HashTable<double,double>[L];
	for (int i=0; i <= L - 1; i++)
	{
		hashtable[i].HashTable_Init(i, var_k, dimensions, tsize, 'e', 4);
	}
}

Euclidean::~Euclidean()
{
	for (int i = 0; i <= L-1; i++)
		delete[] random[i];
	delete[] random;
	delete[] hashtable;
	cout << "Delete Euclidean class" << endl;
}

int Euclidean::get_L(){
	return L;
}


void Euclidean::Euclidean_Reader(char *line, int Hash_index, int Number){
	int line_count = 0;
	char str[20];
	int j, i=0;
	double *coordinate;
	coordinate = new double[dims];
	Node<double>* node;
	while(line[line_count] != '\t')
		line_count++;
	line_count++;
		
//Read all coordinates from pointer
	for(int i=0; i <= dims - 1; i++)
	{
		j=0;
		while(line[line_count] != '\t') 
		{
			if (line[line_count] == '\0')
				break;
			str[j]= line[line_count];
			line_count++;
			j++;
		}
		str[j] = '\0';
		coordinate[i] = atof(str);
		line_count++;
	}
	
	node = new Node<double>(coordinate, dims, Number);
	
	hashtable[Hash_index].Insert_Node(node, random);
}


void Euclidean::set_R(double varR)
{
	R = varR;
}

void Euclidean::set_c(double varc)
{
	c = varc;
}

void Euclidean::Euclidean_LSH(char *line, ofstream& output, int Number){  //Dont care to create one node for all hashtables 
	double time_LSH, time_ALL;
	double dist;
	int line_count = 0;
	char str[20];
	int j, i=0;
	double *coordinate;
	coordinate = new double[dims];
	
	Node<double>* node;	
	
	List<double>* qlist;
	List<double>* final_list_LSH;
	List<double>* final_list_ALL;
	
	while(line[line_count] != '\t')
		line_count++;
	line_count++;
		
	//Read all coordinates from pointer
	for(int i=0; i <= dims - 1; i++)
	{
		j=0;
		while(line[line_count] != '\t') 
		{
			if (line[line_count] == '\0')
				break;
			str[j]= line[line_count];
			line_count++;
			j++;
		}
		str[j] = '\0';
		coordinate[i] = atof(str);
	//	cout << setprecision(12) << coordinate[i] << "  " << i << endl;
		line_count++;
	}
		
	node = new Node<double>(coordinate, dims, Number);
	
	qlist = new List<double>;
	final_list_LSH = new List<double>;
	final_list_ALL = new List<double>;

	output << "Query: Item";
	output << node->get_Number();
	output << '\n';
	
	//Search closer neighbor and if R > 0 find those with dist > R*c and the closest neighbor
	cout << "item_idS" << Number << '\t';
	const clock_t begin_time_LSH  = clock();						//time	
	for(int i=0; i <= L -1; i++)
	{
		hashtable[i].HashTable_LSH(node, qlist, random);
		final_list_LSH->Insert_List(qlist);
	}
	dist = final_list_LSH->Euclidean_Distance(node, R, c, output, 0);			//0 for LSH
	output << "distanceLSH: ";
	output << dist;
	output << '\n';
		
	time_LSH = (double)((clock() - begin_time_LSH ) /  (double)CLOCKS_PER_SEC);
	
	const clock_t begin_time_ALL = clock();							//time
	hashtable[0].HashTable_Search_All(node, final_list_ALL);
	
	dist = final_list_ALL->Euclidean_Distance(node, R, c, output, 1);			//1 for ALL
	output << "distanceTrue: ";
	output << dist;
	output << '\n';
	time_ALL = (double)(( clock() - begin_time_ALL ) /  (double)CLOCKS_PER_SEC);			//time
	
	output << "tLSH: ";
	output << time_LSH;
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



void Euclidean::printList(){
	for (int i=0; i <= L - 1; i++)
	{
		cout << "Hashtable " << i << endl;
		hashtable[i].printList();
	}
}


