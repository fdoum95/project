#ifndef ClusterEuclidean_H
#define ClusterEuclidean_H

#include <fstream>
#include <iostream>
#include "Euclidean.h"
#include "Cluster.h"
#include "List.h"
#include "Node.h"

class ClusterEuclidean
{
	private:
		int k;			//number of clusters
		int dimensions;
		
		int hash_functions;
		int hash_tables;
		int clarans_franction;
		int clarans_iterations;		
		
		int Number_of_Items;
		bool *Objective_Function_Updated;
		double Objective_Function_Value;
		char optionInit;
		char optionAssign;
		char optionUpdate;
		char metric;
		
		double **Distance_Table;
		List<double> *all_items_list;
		List<double> *clara_list;
		Cluster<double> *cluster;
		
		double *Silhouette_table;
		
		Euclidean *euclidean;
	public:
		ClusterEuclidean(int, int, int, int, int, int);
		~ClusterEuclidean();
		
		int get_ClusterNumber();
		int get_DimensionsNumber();
		
		void ClusterEuclidean_Menu(std::ifstream& infile,std::ifstream& cfile, std::ofstream& outfile);
			
		void Lloyd_Menu(std::ofstream& outfile);
		void Clarans_Menu(std::ofstream& outfile);
		void Clara_Menu(std::ofstream& outfile);
		
		void Euclidean_Reader(char *, int);
		
		void Euclidean_Cluster_Initialization(char);
		
		void Distance_Table_Calculation();
		double Get_Distance(int , int );
		
		void KmedoidsInitialization();
		void ConcentrateInitialization();
		
		double Cluster_Euclidean_Distance(Node<double>* cnode, Node<double>* inode);
		
		void Assignment_Initialization();
		
		void Assignment();
		
		void PAM_Assignment();
		void LSH_Assignment();
		
		bool LSH_Keep_Update(bool *);
				
		bool IsCentroid(Node<double> *);
		
		double Objective_Function();
		
		double Objective_Function_New_Centroid(int ClusterNumber, int ItemClusterNumber, Node<double> *new_centroid);
		
		void Update_Lloyd();
		
		void Update_Lloyd_Change_All_medoids();
		
		void Update_Clarans();
		
		void Update_Clarans_One_Swap();
		
		Node<double> * Cluster_Medoid_Calculation(int);
		bool Keep_Update();
		
		Node<double> * get_second_best_Centroid(int, Node<double> *);
		
		void Clarans_Swap(int new_cluster_number, int new_centroid_cluster_number, Node<double> *new_centroid);
		
		void printClusters();
		
		void Clara_Function();
		
		void Clara_New_Sample(int NumItems);
		
		void PAM_Update();
		
		void Silhouette(std::ofstream& outfile);
};

#endif
