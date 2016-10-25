#ifndef Bucket_H
#define Bucket_H

#include "list.h"

class Bucket
{
    private:
    	int *Hfunctions;
    	List<char> *str_list;
    	List<int> *int_list;
    public:
        Bucket();
        ~Bucket();

        int get_size();
        int get_nBuckets();
        
        void Hfunctions_Init(int , int, int);
};

#endif

