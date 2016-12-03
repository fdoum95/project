#include <fstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <time.h>
#include "Random.h"
#include "ClusterDistanceMatrix.h"
#include "Cluster.h"
#include "List.h"
#include "Node.h"

using namespace std;

ClusterDistanceMatrix::ClusterDistanceMatrix(int k_value, int num_items, int hfunctions, int htables, int cfraction, int citerations)
{
	cout << "A Cluster DistanceMatrix class was created." << endl;
	nItems = num_items;
	k = k_value;
	
	hash_functions = hfunctions;
	hash_tables = htables;
	clarans_franction = cfraction;
	clarans_iterations = citerations;
	
	metric = 'h';
	
	Objective_Function_Value = 0;
	Objective_Function_Updated = new bool[k];
	for (int i=0; i <= k - 1; i++)	
		Objective_Function_Updated[i] = true;
	
	all_items_list = new List<int>;
	cluster = new Cluster<int>[k];
	for (int i=0; i <= k - 1; i++)
	{
		cluster[i].Cluster_Initialization(i);
	}
}

ClusterDistanceMatrix::~ClusterDistanceMatrix()
{
	delete all_items_list;
	delete [] cluster;
	cout << "Delete Cluster Hamming class" << endl;
}


int ClusterDistanceMatrix::get_ItemsNumber(){
	return nItems;
}

int ClusterDistanceMatrix::get_ClusterNumber(){
	return k;
}

//-------------------------------------------------------------------MENU FUNCTION-----------------------------------------------------------------------------
void ClusterDistanceMatrix::ClusterDistanceMatrix_Menu(ifstream& infile, ifstream& cfile, ofstream& outfile, int **array)
{	
	cout << "ClusterDistanceMatrix" << endl;	
	double time_LSH, time_ALL;
	table = array;
	
	//Fisrt thing is to read all items from infile
	int counter = 0, MAXIMUM = 10000;
	string line;
	getline(infile, line);
	getline(infile, line);
	if (infile.is_open())
	{
		dmatrix = new DistanceMatrix(hash_functions , hash_tables, nItems, table);
		while(getline(infile, line) && counter < MAXIMUM)     //Start reading items and put them at lists
		{
			DistanceMatrix_Reader(counter+1, counter);
			for(int i=0; i <= hash_tables - 1; i++)
				dmatrix->DistanceMatrix_Reader(i, counter+1, counter);		//for LSH/DBH
			counter++;
		}
	}
	
	for(int oInit = 1; oInit <= 2; oInit++)
	{
		for(int oAssign = 1; oAssign <= 2; oAssign++)
		{
			for(int oUpdate = 1; oUpdate <= 2; oUpdate++)
			{
				Node<int> *current = all_items_list->get_start();;
				Node<int> *new_node;
				List<int> *temp_list = new List<int>;
				while(current != NULL)
				{
					new_node = new Node<int>(current->get_data(), current->get_size(), current->get_Number(), current->get_Number_Table());
					temp_list->Insert_Node_End(new_node);
					current = current->get_next();
				}
				
				const clock_t begin_time_LSH = clock();
				if (oInit == 1)
					this->optionInit = '1';
				else
					this->optionInit = '2';
				
				if (oAssign == 1)
					this->optionAssign = '1';
				else
					this->optionAssign = '2';
					
				if (oUpdate == 1)
					this->optionUpdate = '1';
				else
					this->optionUpdate = '2';
				
				cout << "Algorithm: I" << oInit << "A" << oAssign << "U" << oUpdate << ", x = {1,2} " << endl;
				//Initialization
				cout << "Items : " << all_items_list->get_ListSize() << endl;				
				DistanceMatrix_Cluster_Initialization(optionInit);	
				
				outfile << "Algorithm: I" << oInit << "A" << oAssign << "U" << oUpdate << ", x = {1,2} ";
				outfile << '\n';
				
				if (optionUpdate == '1')			//A la Lloyd
					Lloyd_Menu(outfile);
				else
					Clarans_Menu(outfile);
				
				time_LSH = (double)((clock() - begin_time_LSH ) /  (double)CLOCKS_PER_SEC);
				outfile << "clustering_time: " << time_LSH << " //in seconds" << '\n';
				
				outfile << "Silhouette: [";
				for(int i=0; i <= k - 1; i++)
					outfile << Silhouette_table[i] << ",";
				outfile << Silhouette_table[k] << "]" << '\n';
				
				
				delete [] cluster;
				cluster = new Cluster<int>[k];
				
				all_items_list->set_start(temp_list->get_start());
				all_items_list->set_List_size(temp_list->get_ListSize());
				
				temp_list->set_start(NULL);
				delete temp_list;
			}
		}
	}
	//Clara
	Clara_Menu(outfile);
	delete dmatrix;
}


void ClusterDistanceMatrix::Lloyd_Menu(ofstream& outfile)
{
	while(Keep_Update())
		Update_Lloyd_Change_All_medoids();				//or 	 Update_Lloyd()				
	
	Silhouette(outfile);
			
	for(int i=0; i <= k-1; i++)	
		outfile <<  "CLUSTER-" << i << " {size: " << (cluster[i].get_List())->get_ListSize() << ", medoid: item" << (cluster[i].get_Centroid())->get_Number() << "}" << '\n';
}


void ClusterDistanceMatrix::Clarans_Menu(ofstream& outfile)
{
	int Clarans_Objective_Function_Best = 0;
	int *clarans_centroid_ItemNumber = new int[k];
	int *clarans_centroid_ListSize = new int[k];
	for(int s=1; s <= clarans_iterations; s++)
	{
		Update_Clarans_One_Swap();			//or     	Update_Clarans();
		
		if (s == 1)
		{
			Silhouette(outfile);
			for(int i=0; i<=k -1; i++)
			{
				clarans_centroid_ItemNumber[i] = (cluster[i].get_Centroid())->get_Number();
				clarans_centroid_ListSize[i] = (cluster[i].get_List())->get_ListSize();
			}
			Clarans_Objective_Function_Best = Objective_Function_Value;
		}
		
		if (Objective_Function_Value < Clarans_Objective_Function_Best)
		{
			Silhouette(outfile);
			for(int i=0; i<=k -1; i++)
			{
				clarans_centroid_ItemNumber[i] = (cluster[i].get_Centroid())->get_Number();
				clarans_centroid_ListSize[i] = (cluster[i].get_List())->get_ListSize();
			}
			Clarans_Objective_Function_Best = Objective_Function_Value;
		}
		//clear lists
		all_items_list->set_List_size(0);
		List<int> *clusterlist;
		for(int i=0 ; i <= k - 1; i++)					//Put all non centroid items at all_items_list
		{
			clusterlist = cluster[i].get_List();
			all_items_list->Insert_Cluster_List(clusterlist);
			all_items_list->Insert_Node_End(cluster[i].get_Centroid());
			(cluster[i].get_Centroid())->set_notCentroidValue();
			cluster[i].set_List_empty();
		}
	
		if (s < clarans_iterations)
		{		
			if (optionInit == '1')
			{
				cout << "k-medoids++ Initialization" << endl;
				KmedoidsInitialization();
			}
			else if (optionInit == '2')
			{
				cout << "Park-Jun Initialization" << endl;
				ConcentrateInitialization();
			}
		}
	}
	Objective_Function_Value = Clarans_Objective_Function_Best;
						
	for(int i=0; i <= k-1; i++)	
		outfile <<  "CLUSTER-" << i << " {size: " << clarans_centroid_ListSize[i] << ", medoid: item" << clarans_centroid_ItemNumber[i] << "}" << '\n';
	
}


void ClusterDistanceMatrix::Clara_Menu(ofstream& outfile)
{
	int Clara_Objective_Function_Previous = 20000;
	Node<int> *node;
	int *clara_centroid_ItemNumber = new int[k];
	int *clara_centroid_ListSize = new int[k];
	double time_LSH, time_ALL;
	const clock_t begin_time_LSH = clock();
	optionAssign = '1';
	for (int s=0; s<= 4; s++)
	{
		Clara_Function();
					
		if (Objective_Function_Value < Clara_Objective_Function_Previous)
		{
			Silhouette(outfile);
			for(int i=0; i<=k -1; i++)
			{
				node = cluster[i].get_Centroid();
				clara_centroid_ItemNumber[i] = node->get_Number();
				clara_centroid_ListSize[i] = (cluster[i].get_List())->get_ListSize();
			}
				
			Clara_Objective_Function_Previous = Objective_Function_Value;
		}
						
		all_items_list->set_start(clara_list->get_start());
		all_items_list->set_List_size(clara_list->get_ListSize());
		
		clara_list->set_start(NULL);
		clara_list->set_List_size(0);
			
		if (s < 4)
		{
			delete [] cluster;
			cluster = new Cluster<int>[k];
		}
		
		for(int l=0; l <= Number_of_Items -1; l++)
			delete [] Distance_Table[l];
		delete [] Distance_Table;
	}
		
	outfile << "Algorithm: CLARA";
	outfile << '\n';
		
	for(int i=0; i <= k-1; i++)	
		outfile <<  "CLUSTER-" << i << " {size: " << clara_centroid_ListSize[i] << ", medoid: item" << clara_centroid_ItemNumber[i] << "}" << '\n';
	
	delete [] clara_centroid_ItemNumber;
	delete [] clara_centroid_ListSize;
	
	delete [] cluster;
	cluster = new Cluster<int>[k];
	
	time_LSH = (double)((clock() - begin_time_LSH ) /  (double)CLOCKS_PER_SEC);
	outfile << "clustering_time: " << time_LSH << " //in seconds" << '\n';
				
	outfile << "Silhouette: [";
	for(int i=0; i <= k - 1; i++)
		outfile << Silhouette_table[i] << ",";
	outfile << Silhouette_table[k] << "]" << '\n';
}



void ClusterDistanceMatrix::DistanceMatrix_Reader(int ItemNumber, int NumberTable)
{
	int line_count = 0;
	int *dist;
	dist = new int[get_ItemsNumber()];
	
	Node<int>* node;
	
	for(int i=0; i <= get_ItemsNumber() - 1; i++)
		dist[i] = table[NumberTable][i];
	
	
	node = new Node<int>(dist, get_ItemsNumber(), ItemNumber, NumberTable);
	
	all_items_list->Insert_Node_End(node);			//put item at the list
}


//---------------------------------------------------------Initialization-----------------------------------------------------------
void ClusterDistanceMatrix::DistanceMatrix_Cluster_Initialization(char option)
{	
	//Calculate Distance_Table
	Distance_Table_Calculation();
	
	if (option == '1')
	{
		cout << "k-medoids++ Initialization" << endl;
		KmedoidsInitialization();
	}
	else if (option == '2')
	{
		cout << "Park-Jun Initialization" << endl;
		ConcentrateInitialization();
	}
}

//Dhmiourgei enan anw trigwniko pinaka me tis apostaseis
void ClusterDistanceMatrix::Distance_Table_Calculation()
{
	int number_of_items = all_items_list->get_ListSize();
	Number_of_Items = number_of_items;
	int line = 0, column = 0;
	Node<int> *current_node_line = all_items_list->get_start();
	Node<int> *current_node_column;
	
	Distance_Table = new double*[number_of_items];
	for(int i=0; i<= number_of_items - 1; i++)
		Distance_Table[i] = new double[number_of_items];
	
	line=0;
	while(current_node_line != NULL)
	{
		current_node_column = current_node_line;
		column = line;
		while(current_node_column != NULL)
		{
			Distance_Table[line][column] = table[line][column];
			column++;
			current_node_column = current_node_column->get_next();
		}
		line++;
		current_node_line = current_node_line->get_next();
	}
}



int ClusterDistanceMatrix::Cluster_DistanceMatrix_Distance(Node<int>* cnode, Node<int>* inode)
{
	int i,j;
	i = cnode->get_Number_Table();
	j = inode->get_Number_Table();
	if (i <= j)
		return Distance_Table[i][j];
	else
		return Distance_Table[j][i];
}


//Return the distance of two items from the Distance_Table (instead of calculating again)
double ClusterDistanceMatrix::Get_Distance(int i, int j)
{
	if (i <= j)
		return Distance_Table[i][j];
	else
		return Distance_Table[j][i];
}


void ClusterDistanceMatrix::KmedoidsInitialization()
{
	int random = 0, number_of_items = all_items_list->get_ListSize(), cluster_count = 0;

//1st Step : Choose a random centroid
	random = uniform_distribution(0, number_of_items-1);
	Node<int> *centroid = all_items_list->get_random_Node(random);
	cluster[0].set_Centroid(centroid);			//set the first centroid randomly
	
//2nd step, 3, 4
	float *P_Table = new float[number_of_items];
	float **Distance;
	float *final_Distance = new float[number_of_items];
	float sum = 0, float_random;
	int j=0, Dmax = 0, x = 0, hamming_dist;
		
	Distance = new float*[get_ClusterNumber()];
	for(int i=0; i <= get_ClusterNumber()-1; i++)
		Distance[i] = new float[number_of_items];
	
	Node<int> *current = all_items_list->get_start();	

	while(cluster_count < get_ClusterNumber()-1)
	{		
		current = all_items_list->get_start();
		j=0;
		Dmax = 0;
		sum = 0;
		x = 0;		
		while(current != NULL)
		{
			hamming_dist = Get_Distance((cluster[cluster_count].get_Centroid())->get_Number_Table(), current->get_Number_Table());
			Distance[cluster_count][j] = hamming_dist;
			j++;
			current = current->get_next();
		}
		
		//This is used to avoid computing distances between centroids, make their value equal to zero
		for(int j=0; j <= number_of_items-1; j++)
			for(int i=0; i <= cluster_count; i++)
				if (Distance[i][j] == 0)
					for(int item=0; item <= cluster_count; item++)
						Distance[item][j] = 0;
		
		//Estimation of minimal distance 
		for(int i=0; i <= cluster_count; i++)
		{
			if (i == 0)				//Initialization for Distance with the first cluster
			{
				for(int j=0; j <= number_of_items-1; j++)
					final_Distance[j] = Distance[i][j];	
			}
			else
			{
				for(int j=0; j <= number_of_items-1; j++)
					if (final_Distance[j] > Distance[i][j])
						final_Distance[j] = Distance[i][j];
			}
		}
		
		//Estimate max at Distance Table
		for(int j=0; j <= number_of_items-1; j++)
		{
			if (final_Distance[j] > Dmax)
				Dmax = final_Distance[j];
		}
		
		//Divide each element with Dmax
		for(int j=0; j <= number_of_items-1; j++)
			final_Distance[j] /= Dmax;
		
		//Estimate P(r) table (value is zero for the distance from c to c' (c,c' from C))
		for(int j=0; j <= number_of_items-1; j++)
		{
			P_Table[j] = pow(final_Distance[j], 2) + sum;
			sum = P_Table[j];
		}
		
		do{
			float_random = uniform_distribution_float(0, sum);
			for(int j=0; j <= number_of_items-1; j++)
			{
				if (float_random <= P_Table[j])		//Check every single element. Not need to check if P_Table[j-1] < float_random because it is already checked as P_table[j-1] is checked a loop before . We can use binary search instead
				{
					x = j;
					break;
				}
			}
			centroid = all_items_list->get_random_Node(x);
		}while(centroid->get_CentroidValue());						//Check if the centroid is already used	
		
		cluster_count++;
		
		cluster[cluster_count].set_Centroid(centroid);
	}
	
	//Assignment items with centroid value = false from all_items_list to new centroids
	Assignment_Initialization();
	
	//Print
//	printClusters();
	
	for(int i=0; i <= get_ClusterNumber()-1; i++)
		delete [] Distance[i];
	delete [] Distance;
	delete [] P_Table;
	delete [] final_Distance;
}


void ClusterDistanceMatrix::ConcentrateInitialization()
{
	//Step 1 : array nxn //already calculated
	int NumItems = Number_of_Items;
	
	//Step 2 : Vi
	double *V_Table = new double[NumItems];
	double *divide_sum = new double[NumItems];
	double sum = 0;
	
	//Calculate for all j one time, less time (+)
	for(int j=0; j <= NumItems - 1; j++)
		for(int t=0; t <= NumItems - 1; t++)
			divide_sum[j] += Get_Distance(j, t);
	
	for(int i=0; i <= NumItems - 1; i++)
	{
		sum = 0;
		for(int j=0; j <= NumItems - 1; j++)
			sum += (double)(Get_Distance(i, j) / divide_sum[j]) ;
		V_Table[i] = sum;
	}
	
//Step 3 : find the k minimal Vi
	double min = 100;
	int *Index = new int[k];
	int min_index = 0;
	
	for(int kCluster = 0; kCluster <= k - 1; kCluster++)
	{
		min = 100;
		for(int i=0; i <= NumItems - 1; i++)
		{
			if (V_Table[i] < min && V_Table[i] != 0)
			{
				min = V_Table[i];
				min_index = i;
			}
		}
		V_Table[min_index] = 0;
		Index[kCluster] = min_index;
	}
	
//Create Centroid with Index array
	Node<int> *centroid;
	for(int kCluster = 0; kCluster <= k - 1; kCluster++)
	{
		centroid = all_items_list->get_random_Node(Index[kCluster]);
		cluster[kCluster].set_Centroid(centroid);
	}
	
	//Assignment items with centroid_value = false from all_items_list to new centroids
	Assignment_Initialization();					//It will also estimate Objective Function Value		//Only for the Initialization
	
	//Print
//	printClusters();	
	
	delete [] V_Table;
	delete [] divide_sum;
	delete [] Index;
}


//----------------------------------------------------------------------Assignment----------------------------------------------------------------------
void ClusterDistanceMatrix::Assignment_Initialization()				//It assigns both the centroid and the non centroids items
{
	int min = 10000, min_index = 0;
	Node<int> *min_centroid;
	Node<int> *current = all_items_list->get_start();
	while(current != NULL)
	{
		if (current->get_CentroidValue())
		{
			all_items_list->set_start(current->get_next());
			current->set_next(NULL);
			current = all_items_list->get_start();
		}
		else
		{
			//Calculate k distances from each centroid
			int *distance = new int[k];
			all_items_list->set_start(current->get_next());
			for(int i=0; i <= k - 1; i++)
				distance[i] = Cluster_DistanceMatrix_Distance(cluster[i].get_Centroid(), current);			//CHECK THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				
			min = distance[0];
			min_index = 0;
			
			for(int i=0; i <= k - 1; i++)
			{
				if (distance[i] < min)
				{
					min = distance[i];
					min_index = i;
					min_centroid = cluster[i].get_Centroid();
				}
			}
			
			current->set_next(NULL);
			cluster[min_index].Cluster_Assignment(current);
			current = all_items_list->get_start();
			delete [] distance;
		}
	}
	Objective_Function_Value = Objective_Function();			//Only for the Initialization
	cout << "Initial Objective Function is " << Objective_Function_Value << endl;
}


void ClusterDistanceMatrix::Assignment()						//It assign only the non centroid items
{
	int min = 10000, min_index = 0;
	Node<int> *min_centroid;
	Node<int> *current = all_items_list->get_start();
	while(current != NULL)
	{
		//Calculate k distances from each centroid
		int *distance = new int[k];
		all_items_list->set_start(current->get_next());
		for(int i=0; i <= k - 1; i++)
			distance[i] = Cluster_DistanceMatrix_Distance(cluster[i].get_Centroid(), current);
			
		min = distance[0];
		min_index = 0;
		
		for(int i=0; i <= k - 1; i++)
		{
			if (distance[i] < min)
			{
				min = distance[i];
				min_index = i;
				min_centroid = cluster[i].get_Centroid();
			}
		}
		
		current->set_next(NULL);
		cluster[min_index].Cluster_Assignment(current);
		current = all_items_list->get_start();
		delete [] distance;
	}
}


void ClusterDistanceMatrix::PAM_Assignment()
{
//	cout << "PAM Assignment" << endl;
	List<int> *clusterlist;
	for(int i=0 ; i <= k - 1; i++)					//Put all non centroid items at all_items_list
	{
		clusterlist = cluster[i].get_List();
		all_items_list->Insert_Cluster_List(clusterlist);
		cluster[i].set_List_empty();
	}
	
	Assignment();									//assign all the items in all_items_list (they are only non centroid items)
	
	//Print
//	printClusters();	
}


void ClusterDistanceMatrix::LSH_Assignment()
{
//	cout << "LSH Assignment" << endl;
	
	List<int> *clusterlist;
	for(int i=0 ; i <= k - 1; i++)			//put all items out of the list like PAM
	{
		clusterlist = cluster[i].get_List();
		all_items_list->Insert_Cluster_List(clusterlist);
		cluster[i].set_List_empty();
	}
	
	List<int>* LSH_centroid_list;
	LSH_centroid_list = new List<int>[k];
	
	for(int i=0; i <= k - 1; i++)
		LSH_centroid_list[i].set_start((dmatrix->LSH_Cluster_DistanceMatrix(cluster[i].get_Centroid()))->get_start());			//LSH cluster Hamming is made in class Hamming
	
	
	Node<int> *c_current;
	Node<int> *c_previous;
	Node<int> *c_deleted_node;
	
	//Make the lists from LSH Hashtables
	for(int i=0; i <= k - 1; i++)
	{
		c_current = LSH_centroid_list[i].get_start();
		c_previous = LSH_centroid_list[i].get_start();
		while(c_current != NULL)
		{
			if (IsCentroid(c_current))
			{
				c_deleted_node = c_current;
				if (c_previous == c_current)			//1st at list
					LSH_centroid_list[i].set_start(c_current->get_next());
				else
					c_previous->set_next(c_current->get_next());
				
				c_current = c_current->get_next();
				delete c_deleted_node;
			}
			else
			{
				c_previous = c_current;
				c_current = c_current->get_next();
			}
		}
	}

//Generate R (Initialize it)	
	int Rmin = Cluster_DistanceMatrix_Distance(cluster[0].get_Centroid(), cluster[1].get_Centroid());
	int dist;
	for(int i=0; i <= k - 1; i++)
	{
		for(int j = i+1; j <= k - 1; j++)
		{
			dist = Cluster_DistanceMatrix_Distance(cluster[i].get_Centroid(), cluster[j].get_Centroid());
			if (dist < Rmin)
			{
				Rmin = dist;
			}
		}
	}
	Rmin = Rmin / 2;
	//cout << "R min is " << Rmin << endl;					
					
	Node<int> *current;
	Node<int> *previous;
	Node<int> *del_node;					
	Node<int> *item;
					
	Node<int> *current_list;	//from other list
	Node<int> *previous_list;
	Node<int> *del_node_list;
	
	int itemNumber, cluster_number = 0, Rtemp, R = Rmin;
	double distance = 0;
	
	bool *flag;
	flag = new bool[k];
	for(int fi=0; fi <= k -1; fi++)
		flag[fi] = true;
		
	
	while(LSH_Keep_Update(flag))					//Gia kapoio R
	{
	//	cout << "R is " << R << endl;
		for(int fi=0; fi <= k -1; fi++)
			flag[fi] = false;
		
		for(int i=0; i <= k -1; i++)				//Gia ka8e centroid
		{
			current = LSH_centroid_list[i].get_start();
			previous = LSH_centroid_list[i].get_start();
			
			while(current != NULL)					//Gia ka8e item
			{
				distance = Get_Distance((cluster[i].get_Centroid())->get_Number_Table(), current->get_Number_Table());
				if (distance <= R)
				{
					flag[i] = true;
					
					Rtemp = distance;				//temporary value for R until we check all lists
					item = current;
					itemNumber = current->get_Number();
					cluster_number = i;						//keep centroid number for this item
						
					if (previous == current)			//1st at list
					{
						LSH_centroid_list[i].set_start(current->get_next());			//Delete it
						previous = current->get_next();
					}
					else
						previous->set_next(current->get_next());						//Delete it
					
					for(int j=0; j <= k - 1; j++)		//Check all other lists for the same item and see if there is a centroid that is closer than the centroid of the i cluster 
					{
						if (i != j)
						{
							current_list = LSH_centroid_list[j].get_start();
							previous_list = LSH_centroid_list[j].get_start();
							
							while(current_list != NULL)
							{
								if (itemNumber == current_list->get_Number())
								{
									del_node_list = current_list;
									if (previous_list == current_list)			//1st at list
									{
										LSH_centroid_list[j].set_start(current_list->get_next());
										previous_list = current_list->get_next();
									}
									else
										previous_list->set_next(current_list->get_next());
									
									distance = Get_Distance((cluster[j].get_Centroid())->get_Number_Table(), current_list->get_Number_Table());
									if (distance < Rtemp)
									{
										Rtemp = distance;
										cluster_number = j;
									}
									
									current_list = current_list->get_next();
									delete del_node_list;
								}
								else
								{
									previous_list = current_list;
									current_list = current_list->get_next();
								}							
							}
						}
					}
					//Find it at all items list
					Node<int> *current_all_list = all_items_list->get_start();
					Node<int> *previous_all_list = all_items_list->get_start();
					while(current_all_list != NULL)
					{
						if (current_all_list->get_Number() == itemNumber)
						{
							if (previous_all_list == current_all_list)								//1st at list
							{
								all_items_list->set_start(current_all_list->get_next());			//Delete it from all_items_list
								previous_all_list = current_all_list->get_next();					
							}
							else
								previous_all_list->set_next(current_all_list->get_next());			//Delete it from all_items_list
							
							current_all_list->set_next(NULL);
							all_items_list->set_List_size(all_items_list->get_ListSize()-1);
							cluster[cluster_number].Cluster_Assignment(current_all_list);			//Assign it to the cluster[centroid number]
							
							current_all_list = NULL;
						}
						else
						{
							previous_all_list = current_all_list;
							current_all_list = current_all_list->get_next();
						}
					}
					current = current->get_next();
					delete item;																	//	delete item;
				}
				else
				{
					previous = current;
					current = current->get_next();
				}
			}
		}
		R = R*2;
	}
		
	Assignment(); 		//Gia ka8e remaining item stin all_items_list 			//Find the closest centroid			//Assign it to the cluster[centroid number]	   		//Delete it from all_items_list
	
	//printClusters();			//Print
}

void ClusterDistanceMatrix::printClusters()
{
	for(int i=0; i <= k -1; i++)
	{
		cout << "Cluster " << i << " with centroid: ";
		cluster[i].printCentroid();
		cout << endl;
		cluster[i].printList();
	}
	cout << "Objective  Value   J = " << Objective_Function_Value << endl;
}


bool ClusterDistanceMatrix::LSH_Keep_Update(bool *flag)
{
	int count = 0;					//number of cluster that has at least one item assign from the bucket
	for(int i=0; i <= k - 1; i++)
		if (flag[i])
			count++;
	if (count > k/2)				//more than the half
		return true;
	else
		return false;
}

//----------------------------------------------------------------------Objective_Function----------------------------------------------------------------------

long int ClusterDistanceMatrix::Objective_Function()
{
	long int sum = 0;
	Node<int> *centroid;
	Node<int> *current;
	List<int> *items_list;
	for(int i =0; i <= k -1 ; i++)
	{
		centroid = cluster[i].get_Centroid();
		items_list = cluster[i].get_List();
		if (items_list->get_start() != NULL)
		{
			current = items_list->get_start();
			while(current != NULL)
			{
				sum += Get_Distance(centroid->get_Number_Table(), current->get_Number_Table());
				current = current->get_next();
			}
		}
	}
	return sum;
}


int ClusterDistanceMatrix::Objective_Function_New_Centroid(int ClusterNumber, int ItemClusterNumber, Node<int> *new_centroid)
{
	int sum = 0;
	double distance = 0;
	Node<int> *previous_centroid;
	Node<int> *centroid;
	Node<int> *second_best_Centroid;
	Node<int> *current;
	List<int> *items_list;
	for(int i =0; i <= k -1 ; i++)
	{
		if (i == ClusterNumber)
		{
			previous_centroid = cluster[i].get_Centroid();
			items_list = cluster[i].get_List();		//there is not here the cluster[i].get_Centroid(), so will calculate it later
			
			current = items_list->get_start();
			while(current != NULL)
			{
				second_best_Centroid = get_second_best_Centroid(ClusterNumber, current);
				if (Get_Distance(new_centroid->get_Number_Table(), current->get_Number_Table()) <=  Get_Distance(second_best_Centroid->get_Number_Table(), current->get_Number_Table()))
				{
					sum += Get_Distance(new_centroid->get_Number_Table(), current->get_Number_Table());
				}
				else
				{
					sum += Get_Distance(second_best_Centroid->get_Number_Table(), current->get_Number_Table());
				}
				current = current->get_next();
			}
			
			
			//this for the previous centroid m that is faced as a non-centroid
			second_best_Centroid = get_second_best_Centroid(ClusterNumber, previous_centroid);				//the first best centroid is itself, so find the second from the other k-1 clusters
			if (Get_Distance(new_centroid->get_Number_Table(), previous_centroid->get_Number_Table()) <=  Get_Distance(second_best_Centroid->get_Number_Table(), previous_centroid->get_Number_Table()))			//Compare second best with new centroid t
			{
				sum += Get_Distance(new_centroid->get_Number_Table(), previous_centroid->get_Number_Table());
			}
			else
			{
				sum += Get_Distance(second_best_Centroid->get_Number_Table(), previous_centroid->get_Number_Table());
			}
		}
		else
		{
			centroid = cluster[i].get_Centroid();
			items_list = cluster[i].get_List();		//there is not here the cluster[i].get_Centroid(), so will calculate it later
			
			current = items_list->get_start();
			while(current != NULL)
			{	
				if (Get_Distance(new_centroid->get_Number_Table(), current->get_Number_Table()) <=  Get_Distance(centroid->get_Number_Table(), current->get_Number_Table()))
				{
					sum += Get_Distance(new_centroid->get_Number_Table(), current->get_Number_Table());
				}
				else
				{
					sum += Get_Distance(centroid->get_Number_Table(), current->get_Number_Table());
				}
				current = current->get_next();
			}
			if (i == ItemClusterNumber)									//Dont calculate the distance between new centroid and its centroid before the swap, only for Clarans
				sum -= Get_Distance(centroid->get_Number_Table(), new_centroid->get_Number_Table());
		}
	}
	return sum;
}

bool ClusterDistanceMatrix::IsCentroid(Node<int> *node)
{
	for(int i=0; i <= k-1; i++)
	{
		if (node->get_Number() == (cluster[i].get_Centroid())->get_Number())
			return true;
	}
	return false;
}


Node<int> * ClusterDistanceMatrix::get_second_best_Centroid(int NumCluster, Node<int> *node)
{
	int min_index = 0;
	int min = 10000;
	int dist = 0;
	for(int i = 0; i <= k -1 ; i++)
	{
		if (i != NumCluster)
		{
			dist = Get_Distance((cluster[i].get_Centroid())->get_Number_Table(), node->get_Number_Table());
			if (dist < min)
			{
				min_index = i;
				min = dist;
			}
		}
	}
	return cluster[min_index].get_Centroid();
}


Node<int> * ClusterDistanceMatrix::Cluster_Medoid_Calculation(int NumCluster)
{
	List<int> *items_list = cluster[NumCluster].get_List();	
	Node<int> *current;
	Node<int> *centroid = cluster[NumCluster].get_Centroid();
	if (items_list->is_empty())
	{
		cout << "Problem: empty list" << endl;
		return NULL;	
	}
	
	long int min;
	int min_index = 0;
	long int *Sum = new long int[items_list->get_ListSize() + 1];
	
	for(int i = 0; i <= items_list->get_ListSize(); i++)
		Sum[i] = 0;
	
	int line = 0;
	Node<int> *current_node_line = items_list->get_start();	//current_node_line is t	
	Node<int> *current_node_column;							//current_node_line is i
	while(current_node_line != NULL)	
	{
		current_node_column = items_list->get_start();
		while(current_node_column != NULL)
		{
			Sum[line] += Get_Distance(current_node_line->get_Number_Table(), current_node_column->get_Number_Table());
			current_node_column = current_node_column->get_next();
		}
		Sum[line] += Get_Distance(current_node_line->get_Number_Table(), centroid->get_Number_Table());				//+ the distance from the center of the cluster
		line++;
		current_node_line = current_node_line->get_next();
	}
	
	//the same thing for the centoid. The last item in array Sum is the Distances for centroid
	current_node_column = items_list->get_start();
	while(current_node_column != NULL)
	{
		Sum[line] += Get_Distance(centroid->get_Number_Table(), current_node_column->get_Number_Table());
		current_node_column = current_node_column->get_next();
	}
	
	//find the min in Sum
	min = Sum[0];
	for(int i = 1; i <= items_list->get_ListSize() ; i++)		//for(int i = 1; i <= items_list->get_ListSize() + 1 - 1; i++)
	{
		if (Sum[i] < min)
		{
			min = Sum[i];
			min_index = i;
		}
	}
	
	if (min_index == items_list->get_ListSize())				//medoid t is already centroid
		return centroid;
	else
		return (items_list->get_random_Node(min_index));		//find this item and return it
	
	delete [] Sum;	
}


bool ClusterDistanceMatrix::Keep_Update()
{
	for(int i = 0; i <= k - 1; i++)
		if (Objective_Function_Updated[i])
			return true;
	return false;
}


void ClusterDistanceMatrix::Update_Lloyd_Change_All_medoids()
{
	float new_ObjFunction_Value = 0;
	int ItemNumber = 0;
	int ItemClusterNumber = 0;
	Node<int> *medoid_centroid;
	Node<int> *new_centroid;
	Node<int> *previous_centroid;
	List<int> *temporary_list;
	temporary_list = new List<int>[k];
	
	for(int i =0 ; i<= k - 1; i++)
		Objective_Function_Updated[i] = false;
	
	for(int i =0 ; i<= k - 1; i++)
	{
		//Calculate new centroid t
		temporary_list[i].set_start(cluster[i].get_Centroid());
		medoid_centroid = Cluster_Medoid_Calculation(i);
		new_centroid = medoid_centroid;
		if (new_centroid != cluster[i].get_Centroid() && new_centroid != NULL)				
		{
			//Calculate New Objective Function
			new_ObjFunction_Value = Objective_Function_New_Centroid(i, i ,new_centroid);
			
			//Swap
			cluster[i].Swap(new_centroid);
		}	
	}		
	
	if (optionAssign == '1')				//chosen Assignment
		PAM_Assignment();
	else									//chosen Assignment
		LSH_Assignment();
	new_ObjFunction_Value = Objective_Function();
	
	if (new_ObjFunction_Value < Objective_Function_Value)			//if J' < J
	{							//swap -> set new centroid, put previous centroid at items list
		Objective_Function_Updated[0] = true;					
		Objective_Function_Value = new_ObjFunction_Value;
	}
	else
	{
		for(int i =0 ; i<= k - 1; i++)
		{
			previous_centroid = temporary_list[i].get_start();
			ItemNumber = previous_centroid->get_Number();
			//Find item's cluster
			int j=0;
			while(j <= k-1)
			{
				new_centroid = (cluster[j].get_List())->get_Node(ItemNumber);
			
				if (new_centroid != NULL)													//Found it
				{
					ItemClusterNumber = j;
					j=k;
				}
				j++;
			}
			Clarans_Swap(i, ItemClusterNumber, previous_centroid);
		}
		if (optionAssign == '1')				//chosen Assignment
			PAM_Assignment();
		else									//chosen Assignment
			LSH_Assignment();
	}
}


void ClusterDistanceMatrix::Update_Lloyd()
{
	float new_ObjFunction_Value = 0;
	Node<int> *medoid_centroid;
	Node<int> *new_centroid;
	
	for(int i =0 ; i<= k - 1; i++)
		Objective_Function_Updated[i] = false;
	
	for(int i =0 ; i<= k - 1; i++)
	{
		//Calculate new centroid t
		medoid_centroid = Cluster_Medoid_Calculation(i);
		new_centroid = medoid_centroid;
		if (new_centroid != cluster[i].get_Centroid() && new_centroid != NULL)				
		{
			//Calculate New Objective Function
			new_ObjFunction_Value = Objective_Function_New_Centroid(i, i ,new_centroid);
			
			//Swap
			if (new_ObjFunction_Value < Objective_Function_Value)			//if J' < J
			{
				cluster[i].Swap(new_centroid);							//swap -> set new centroid, put previous centroid at items list
				Objective_Function_Updated[i] = true;					
				Objective_Function_Value = new_ObjFunction_Value;
				if (optionAssign == '1')				//chosen Assignment
					PAM_Assignment();
				else									//chosen Assignment
					LSH_Assignment();
			}
		}		
	}
}



void ClusterDistanceMatrix::Update_Clarans()
{
	int Qnum = 0, nonCentroid_number = 0;
	//plh8ari8mos tou Q (|Q|)
	for(int i=0; i <= k -1; i++)
		nonCentroid_number += (cluster[i].get_List())->get_ListSize();
	Qnum = nonCentroid_number * k * (0.12);
	if (Qnum < 250)
		Qnum = 250;
	
	int random_x;
	int Q[Qnum][2];
	
	for(int i = 0; i <= Qnum - 1; i++)
	{
		random_x = uniform_distribution(0, (nonCentroid_number+k)*k - 1);
		Q[i][0] = random_x % k;
		Q[i][1] = floor(random_x / k);	
		//cout << "(" << Q[i][0] << "," << Q[i][1] << ")" << '\t';
	}
	
	Node<int> *new_centroid;
	int new_ObjFunction_Value, ClusterNumber = 0, ItemClusterNumber = 0, ItemNumber = 0;
	for(int index = 0; index <= Qnum - 1; index++)
	{
		ClusterNumber = Q[index][0];			//set cluster from Q[index][0]
		ItemNumber = Q[index][1];				//set item from Q[index][1]
		//Find item's cluster
		int j=0;
		while(j <= k-1)
		{
			new_centroid = (cluster[j].get_List())->get_Node_Table_Number(ItemNumber);
			if (new_centroid == NULL)						//If it is not at the list , then it could be the centroid of the cluster
				if (ItemNumber == (cluster[j].get_Centroid())->get_Number_Table())			//It is the centroid
					new_centroid = cluster[j].get_Centroid();
			
			if (new_centroid != NULL)													//Found it
			{
				ItemClusterNumber = j;
				j=k;
			}
			j++;
		}
		
		if (new_centroid != cluster[ClusterNumber].get_Centroid() && new_centroid != NULL)		//if it is no changes			//	new_centroid != cluster[new_centroid_cluster_number].get_Centroid() && new_centroid != NULL
		{
			new_ObjFunction_Value = Objective_Function_New_Centroid(ClusterNumber, ItemClusterNumber, new_centroid);			//Calculate New Objective Function
			
			if (new_ObjFunction_Value < Objective_Function_Value)			//if J' < J
			{
				Clarans_Swap(ClusterNumber, ItemClusterNumber, new_centroid);							//swap -> set new centroid, put previous centroid at items list				
				Objective_Function_Value = new_ObjFunction_Value;										//Update Objective Function
				if (optionAssign == '1')			//chosen Assignment 
					PAM_Assignment();
				else								//chosen Assignment 
					LSH_Assignment();	 
			}
		}
	}	
}



void ClusterDistanceMatrix::Update_Clarans_One_Swap()
{
	int Qnum = 0, nonCentroid_number = 0;
	//plh8ari8mos tou Q (|Q|)
	for(int i=0; i <= k -1; i++)
		nonCentroid_number += (cluster[i].get_List())->get_ListSize();
	Qnum = nonCentroid_number * k * (0.12);
	if (Qnum < 250)
		Qnum = 250;
	
	int random_x;
	int Q[Qnum][3];
	
	for(int i = 0; i <= Qnum - 1; i++)
	{
		random_x = uniform_distribution(0, (nonCentroid_number+k)*k - 1);
		Q[i][0] = random_x % k;
		Q[i][1] = floor(random_x / k);	
		//cout << "(" << Q[i][0] << "," << Q[i][1] << ")" << '\t';
	}
	
	bool is_a_centroid = false;
	Node<int> *new_centroid;
	int new_ObjFunction_Value, ClusterNumber = 0, ItemClusterNumber = 0, ItemNumber = 0;
	for(int index = 0; index <= Qnum - 1; index++)
	{
		is_a_centroid = false;
		ClusterNumber = Q[index][0];			//set cluster from Q[index][0]
		ItemNumber = Q[index][1];				//set item from Q[index][1]
		//Find item's cluster
		int j=0;
		while(j <= k-1)
		{
			new_centroid = (cluster[j].get_List())->get_Node_Table_Number(ItemNumber);
			if (new_centroid == NULL)
			{											//If it is not at the list , then it could be the centroid of the cluster
				if (ItemNumber == (cluster[j].get_Centroid())->get_Number_Table())			//It is the centroid
				{
					new_centroid = cluster[j].get_Centroid();
					is_a_centroid = true;
				}
			}
			
			if (new_centroid != NULL)													//Found it
			{
				ItemClusterNumber = j;
				j=k;
			}
			j++;
		}
		
		if (is_a_centroid)		//if it is no changes			//	new_centroid != cluster[new_centroid_cluster_number].get_Centroid() && new_centroid != NULL
			Q[index][2] = 0;
		else
			Q[index][2] = Objective_Function_New_Centroid(ClusterNumber, ItemClusterNumber, new_centroid);			//Calculate New Objective Function
	}
	
	int min = Q[0][2], min_index = 0;
	for(int i=0; i < Qnum - 1; i++)
	{
		if (Q[i][2] < min && Q[i][2] != 0)
		{
			min = Q[i][2];
			min_index = i;
		}
	}
	
	ClusterNumber = Q[min_index][0];			//set cluster from Q[index][0]
	ItemNumber = Q[min_index][1];				//set item from Q[index][1]
	int j=0;
	while(j <= k-1)
	{
		new_centroid = (cluster[j].get_List())->get_Node_Table_Number(ItemNumber);
		if (new_centroid == NULL)													//If it is not at the list , then it could be the centroid of the cluster
			if (ItemNumber == (cluster[j].get_Centroid())->get_Number_Table())			//It is the centroid
				new_centroid = cluster[j].get_Centroid();
		
		if (new_centroid != NULL)													//Found it
		{
			ItemClusterNumber = j;
			j=k;
		}
		j++;
	}
	new_ObjFunction_Value = Objective_Function_New_Centroid(ClusterNumber, ItemClusterNumber, new_centroid);			//Calculate New Objective Function
	
	if (new_ObjFunction_Value < Objective_Function_Value)			//if J' < J
	{
		Clarans_Swap(ClusterNumber, ItemClusterNumber, new_centroid);							//swap -> set new centroid, put previous centroid at items list				
		Objective_Function_Value = new_ObjFunction_Value;										//Update Objective Function
		if (optionAssign == '1')			//chosen Assignment 
			PAM_Assignment();
		else								//chosen Assignment 
			LSH_Assignment();	 
	}
}


void ClusterDistanceMatrix::Clarans_Swap(int ClusterNumber, int ItemClusterNumber, Node<int> *new_centroid)
{
	Node<int> *previous_centroid = cluster[ClusterNumber].get_Centroid();
	Node<int> *current_node = (cluster[ItemClusterNumber].get_List())->get_start();
	Node<int> *previous_node = (cluster[ItemClusterNumber].get_List())->get_start();
	if (previous_centroid != new_centroid)
	{
		while(current_node != NULL)
		{
			//find t in list
			if (current_node == new_centroid)
			{
				if (previous_node != current_node)
				{
					previous_node->set_next(current_node->get_next());		//Delete it from list
				}
				else				//first item at list
				{
					(cluster[ItemClusterNumber].get_List())->set_start(current_node->get_next());				//Delete it from list
				}
				(cluster[ItemClusterNumber].get_List())->set_List_size((cluster[ItemClusterNumber].get_List())->get_ListSize() - 1);
				break;
			}
			previous_node = current_node;
			current_node = current_node->get_next();
		}
		//put past_centroid at the end of its list (we dont care where it will be put at)
		previous_centroid->set_notCentroidValue();
		(cluster[ClusterNumber].get_List())->Insert_Node_End(previous_centroid);
	
		//set t as a centroid
		new_centroid->set_next(NULL);
		cluster[ClusterNumber].set_Centroid(new_centroid);
	}
}

//---------------------------------------------------------------------------CLARA---------------------------------------------------------------------------
void ClusterDistanceMatrix::Clara_Function()
{
//	cout << "Clara" << endl;
	
	int NumItems = 40 + 2*k;
	Number_of_Items = NumItems;
	Clara_New_Sample(NumItems);
	
	//Initialization
	DistanceMatrix_Cluster_Initialization('1');
	
	//Update
	PAM_Update();
	
//	printClusters();
}


void ClusterDistanceMatrix::Clara_New_Sample(int NumItems)
{
	int random = 0;
	int count = 0;
	
	clara_list = new List<int>;
	Node<int> *current;
	Node<int> *previous;
	Node<int> *node;
	List<int> *new_list;
	new_list = new List<int>;
	
	for(int i=0; i <= NumItems-1; i++)
	{
		count = 0;
		do
		{
			random = uniform_distribution(0, all_items_list->get_ListSize() - 1);
			current = all_items_list->get_random_Node(random);
		}while(clara_list->get_Node(current->get_Number()) != NULL);
		
		node = new Node<int>(current->get_data(), current->get_size(), current->get_Number(), i);	
		clara_list->Insert_Node_End(node);
	}
	
	Node<int> *temp;
	int list_size_temp = 0;
	
	temp = clara_list->get_start();
	list_size_temp = clara_list->get_ListSize();
	
	clara_list->set_start(all_items_list->get_start());
	clara_list->set_List_size(all_items_list->get_ListSize());
	
	all_items_list->set_start(temp);
	all_items_list->set_List_size(list_size_temp);
	
}


void ClusterDistanceMatrix::PAM_Update()
{
	int new_ObjFunction_Value = 0;
	Node<int> *centroid;
	Node<int> *current;
	
	for(int i=0; i <= k-1; i++)
	{
		centroid = cluster[i].get_Centroid();
		for(int j=0; j <= k -1; j++)
		{
			current = (cluster[j].get_List())->get_start();
			while(current != NULL)
			{
				new_ObjFunction_Value = Objective_Function_New_Centroid(i, j, current);
				if(new_ObjFunction_Value < Objective_Function_Value)
				{
					Clarans_Swap(i, j, current);													//swap -> set new centroid, put previous centroid at items list				
					Objective_Function_Value = new_ObjFunction_Value;										//Update Objective Function
					PAM_Assignment();
				}
				current = current->get_next();
			}
		}
	}
}

//---------------------------------------------------------------------------Silhouette---------------------------------------------------------------------------
void ClusterDistanceMatrix::Silhouette(std::ofstream& outfile)
{
	double *S = new double[Number_of_Items];
	double *A = new double[Number_of_Items];
	double *B = new double[Number_of_Items];
	double *average_S = new double[k];
	double sum = 0;
	int count =0;
	
	Node<int> *item;
	Node<int> *current;
	Node<int> *centroid;
	List<int> *cluster_list;
	for(int i=0; i <= k - 1; i++)
	{
		cluster_list = cluster[i].get_List();
		centroid = cluster[i].get_Centroid();
		item = cluster_list->get_start();
		while (item != NULL)
		{
			sum = 0;
			count = 0;
			current = cluster_list->get_start();
			while(current != NULL)
			{
				sum += Get_Distance(item->get_Number_Table(), current->get_Number_Table());
				count++;
				current = current->get_next();
			}
			sum += Get_Distance(item->get_Number_Table(), centroid->get_Number_Table());
			count++;
			A[item->get_Number_Table()] = sum/count;
			item = item->get_next();
		}
		item = centroid;
		sum = 0;
		count = 0;
		current = cluster_list->get_start();
		while(current != NULL)
		{
			sum += Get_Distance(item->get_Number_Table(), current->get_Number_Table());
			count++;
			current = current->get_next();
		}
		
		if (count != 0)
			A[item->get_Number_Table()] = sum/count;
		else
			A[item->get_Number_Table()] = 0;		
	}
		
	int cluster_number = 0;
	Node<int> *second_best_centroid;
	List<int> *second_cluster_list;
	
	for(int i=0; i <= k - 1; i++)
	{
		cluster_list = cluster[i].get_List();
		centroid = cluster[i].get_Centroid();
		item = cluster_list->get_start();
		while (item != NULL)
		{
			second_best_centroid = get_second_best_Centroid(i, item);
			for(int j = 0; j <= k - 1; j++)
				if (second_best_centroid->get_Number() == (cluster[j].get_Centroid())->get_Number())
					cluster_number = j;
				
			second_cluster_list = cluster[cluster_number].get_List();
			sum = 0;
			count = 0;
			current = second_cluster_list->get_start();
			while(current != NULL)
			{
				sum += Get_Distance(item->get_Number_Table(), current->get_Number_Table());
				count++;
				current = current->get_next();
			}
			sum += Get_Distance(item->get_Number_Table(), second_best_centroid->get_Number_Table());
			count++;
			B[item->get_Number_Table()] = sum/count;
			item = item->get_next();
		}
		
		item = centroid;
		second_best_centroid = get_second_best_Centroid(i, item);
		for(int j = 0; j <= k - 1; j++)
			if (second_best_centroid->get_Number() == (cluster[j].get_Centroid())->get_Number())
				cluster_number = j;
				
		second_cluster_list = cluster[cluster_number].get_List();
		sum = 0;
		count = 0;
		current = second_cluster_list->get_start();
		while(current != NULL)
		{
			sum += Get_Distance(item->get_Number_Table(), current->get_Number_Table());
			count++;
			current = current->get_next();
		}
		if (count != 0)
			B[item->get_Number_Table()] = sum/count;
		else
			B[item->get_Number_Table()] = 0;		
	}
	
	double max;
	for(int i=0; i <= k - 1; i++)
	{
		cluster_list = cluster[i].get_List();
		centroid = cluster[i].get_Centroid();
		item = cluster_list->get_start();
		while (item != NULL)
		{
			if (A[item->get_Number_Table()] < B[item->get_Number_Table()])
				max = B[item->get_Number_Table()];
			else
				max = A[item->get_Number_Table()];
			if (max != 0)
				S[item->get_Number_Table()] = (B[item->get_Number_Table()] - A[item->get_Number_Table()]) / max;
			else
				S[item->get_Number_Table()] = 0;
			item = item->get_next();
		}
		item = centroid;
		if (A[item->get_Number_Table()] < B[item->get_Number_Table()])
			max = B[item->get_Number_Table()];
		else
			max = A[item->get_Number_Table()];
		if (max != 0)
			S[item->get_Number_Table()] = (B[item->get_Number_Table()] - A[item->get_Number_Table()]) / max;
		else
			S[item->get_Number_Table()] = 0;		
	}
	
	for(int i=0; i <= k - 1; i++)
	{
		sum = 0;
		count  = 0;
		cluster_list = cluster[i].get_List();
		centroid = cluster[i].get_Centroid();
		item = cluster_list->get_start();
		while (item != NULL)
		{
			sum += S[item->get_Number_Table()];
			count++;
			item = item->get_next();
		}
		item = centroid;
		sum += S[item->get_Number_Table()];	
		count++;
		if (count != 0)
			average_S[i] = sum / count;
		else
			average_S[i] = 0;
	}
	
	double S_total = 0;
	Silhouette_table = new double[k+1];
	//cout << endl << "Sillouette" << endl;
	for(int i=0; i <= k - 1; i++)
	{
		Silhouette_table[i] = average_S[i];
		S_total += average_S[i];
	}
	Silhouette_table[k] = S_total / k;
	
	delete [] average_S;
	delete [] S;
	delete [] A;
	delete [] B;
}



template class List<int>;
template class List<char>;
template class List<double>;
