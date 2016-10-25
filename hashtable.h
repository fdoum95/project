#ifndef HashTable_H
#define HashTable_H

#include "Bucket.h"

class HashTable
{
    private:
    	int size;
    	Bucket **bucket;
    public:
        HashTable(int, int);
        ~HashTable();

        int get_size();
        int get_nBuckets();
};
#endif

