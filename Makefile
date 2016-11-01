# In  order  to  execute  this  "Makefile " just  type  "make "
OBJS     = main.o Hamming.o Euclidean.o Cosine.o DistanceMatrix.o Hashtable.o Heuristic.o List.o Node.o Random.o
SOURCE   = main.cpp Hamming.cpp Euclidean.cpp Cosine.cpp DistanceMatrix.cpp Hashtable.cpp Heuristic.cpp List.cpp Node.cpp Random.cpp
HEADER   = Hamming.h Euclidean.h Cosine.h DistanceMatrix.h Hashtable.h Heuristic.h List.h Node.h Random.h
OUT      = lsh
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

#  clean  house
clean :
	rm -f $(OBJS) $(OUT)
