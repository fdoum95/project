# In  order  to  execute  this  "Makefile " just  type  "make "
OBJS     = main.o Hamming.o Euclidean.o Cosine.o DistanceMatrix.o Hashtable.o Heuristic.o List.o Node.o Random.o Centroid.o Cluster.o ClusterCosine.o ClusterDistanceMatrix.o ClusterEuclidean.o ClusterHamming.o Space.o
SOURCE   = main.cpp Hamming.cpp Euclidean.cpp Cosine.cpp DistanceMatrix.cpp Hashtable.cpp Heuristic.cpp List.cpp Node.cpp Random.cpp Centroid.cpp ClusterCosine.cpp ClusterDistanceMatrix.cpp ClusterEuclidean.cpp ClusterHamming.cpp Space.cpp
HEADER   = Hamming.h Euclidean.h Cosine.h DistanceMatrix.h Hashtable.h Heuristic.h List.h Node.h Random.h Centroid.h Cluster.h ClusterCosine.h ClusterEuclidean.h ClusterHamming.h Space.h
OUT      = medoids
CC       = g++
FLAGS    = -g -c
# -g  option  enables  debugging  mode
# -c flag  generates  object  code  for  separate  files
all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)
#  create/ compile  the  individual  files  >> separately <<

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

Hamming.o: Hamming.cpp Hamming.h
	$(CC) $(FLAGS) Hamming.cpp

Euclidean.o: Euclidean.cpp Euclidean.h
	$(CC) $(FLAGS) Euclidean.cpp

Cosine.o: Cosine.cpp Cosine.h
	$(CC) $(FLAGS) Cosine.cpp

DistanceMatrix.o: DistanceMatrix.cpp DistanceMatrix.h
	$(CC) $(FLAGS) DistanceMatrix.cpp

Hashtable.o: Hashtable.cpp Hashtable.h
	$(CC) $(FLAGS) Hashtable.cpp

Heuristic.o: Heuristic.cpp Heuristic.h
	$(CC) $(FLAGS) Heuristic.cpp

List.o: List.cpp List.h
	$(CC) $(FLAGS) List.cpp

Node.o: Node.cpp Node.h
	$(CC) $(FLAGS) Node.cpp

Random.o: Random.cpp Random.h
	$(CC) $(FLAGS) Random.cpp

Centroid.o: Centroid.cpp Centroid.h
	$(CC) $(FLAGS) Centroid.cpp

Cluster.o: Cluster.cpp Cluster.h
	$(CC) $(FLAGS) Cluster.cpp

ClusterCosine.o: ClusterCosine.cpp ClusterCosine.h
	$(CC) $(FLAGS) ClusterCosine.cpp

ClusterDistanceMatrix.o: ClusterDistanceMatrix.cpp ClusterDistanceMatrix.h
	$(CC) $(FLAGS) ClusterDistanceMatrix.cpp

ClusterEuclidean.o: ClusterEuclidean.cpp ClusterEuclidean.h
	$(CC) $(FLAGS) ClusterEuclidean.cpp

ClusterHamming.o: ClusterHamming.cpp ClusterHamming.h
	$(CC) $(FLAGS) ClusterHamming.cpp

Space.o: Space.cpp Space.h
	$(CC) $(FLAGS) Space.cpp 

#  clean  house
clean :
	rm -f $(OBJS) $(OUT)
