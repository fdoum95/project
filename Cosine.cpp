#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include "Cosine.h"
#include <time.h>
#include "Node.h"
#include "List.h"

using namespace std;

Cosine::Cosine(int var_k, int var_L, int dimensions)
{
	cout << "A Cosine class was constructed" << endl;
	L = var_L;
	dims = dimensions;
	hashtable = new HashTable<double,double>[L];
	for (int i=0; i <= L - 1; i++)
	{
		hashtable[i].HashTable_Init(i, var_k, dimensions, pow(2, var_k), 'c');
	}
}

Cosine::~Cosine()
{
	delete [] hashtable;
	cout << "Delete Cosine class" << endl;
}

int Cosine::get_L(){
	return L;
}


void Cosine::Cosine_Reader(char *line, int Hash_index, int NumberTable)
{
	int line_count = 0;
	char str[20];
	char NumItem[10];
	int j, it = 0;
	int ItemNumber = 0;
	double *coordinate;
	coordinate = new double[dims];
	Node<double>* node;
	
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
	
	node = new Node<double>(coordinate, dims, ItemNumber, NumberTable);
	
	hashtable[Hash_index].Insert_Node(node);
}



void Cosine::set_R(double varR)
{
	R = varR;
}

void Cosine::set_c(double varc)
{
	c = varc;
}

void Cosine::Cosine_LSH(char *line, ofstream& output, int Number, char option){
	double time_LSH, time_ALL;
	int line_count = 0;
	double dist = 0;
	char str[20];
	int j, i=0;
	double *coordinate;
	int NumberItems = 1000;
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
		line_count++;
	}

//	node = new Node<double>(coordinate, dims, Number);
	
	qlist = new List<double>;
	final_list_LSH = new List<double>;
	final_list_ALL = new List<double>;
	
	output << "Query: Item";
	output << node->get_Number();
	output << '\n';

	if (option == 'y')
		NumberItems = 3*get_L();
	
	//Search closer neighbor and if R > 0 find those with dist > R*c and the closest neighbor
	cout << "item_idS" << Number << '\t';
	const clock_t begin_time_LSH  = clock();						//time	
	for(int i=0; i <= L -1; i++)
	{
		hashtable[i].HashTable_LSH(node, qlist);
		final_list_LSH->Insert_List(qlist, NumberItems);
	}
	
	dist = final_list_LSH->Cosine_Distance(node, R, c, output, 0);			//0 for LSH
	output << "distanceLSH: ";
	output << dist;
	output << '\n';
		
	time_LSH = (double)((clock() - begin_time_LSH ) /  (double)CLOCKS_PER_SEC);
	
	//Search closer neighbor for ALL
	const clock_t begin_time_ALL = clock();							//time
	hashtable[0].HashTable_Search_All(node, final_list_ALL);
	
	dist = final_list_ALL->Cosine_Distance(node, R, c, output, 1);			//1 for True
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

List<double> *Cosine::LSH_Cluster_Cosine(Node<double> *centroid)			//return the final list that is belonged the centroid
{
	List<double>* qlist;
	List<double>* final_list_LSH;
	
	qlist = new List<double>;
	final_list_LSH = new List<double>;
	
	for(int i=0; i <= L -1; i++)
	{
		hashtable[i].HashTable_LSH(centroid, qlist);
		final_list_LSH->Insert_List(qlist);
	}
	
	return final_list_LSH;	
}

void Cosine::printList(){
	for (int i=0; i <= L - 1; i++)
	{
		cout << "Hashtable " << i << endl;
		hashtable[i].printList();
	}
}

