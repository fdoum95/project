#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

void File_Reader(string& line);

int main(int argc, char *argv[])
{ 
  int k = 4;
  int L = 5;
  string line;
  ifstream InFile;
  ifstream QueryFile;
  ofstream OutFile;
  
  cout << "Start of the program." << endl;
  
  if (argc < 6)   //at least we need 3 files 
  {
    cout << "Too few arguments" << endl;
    return - 1;
  }

  for(int i = 0; i < argc; i++)
  {
    if(strcmp(argv[i], "-d") == 0)
    {
      InFile.open(argv[i+1]);
    }
    else if(strcmp(argv[i], "-q") == 0)
    {
      QueryFile.open(argv[i+1]);
    }
    else if(strcmp(argv[i], "-k") == 0)
    {
      k = atoi(argv[i+1]);
    }
    else if(strcmp(argv[i], "-L") == 0)
    {
      L = atoi(argv[i+1]);
    }
    else if(strcmp(argv[i], "-o") == 0)
    {
      OutFile.open(argv[i+1]);
    }
  }

  if (k < 0 || L < 0)
  {
    cout << "Negative arguments" << endl;
    return -1;
  }
  
  cout << "all ok" << endl;
  //read from Input File
  if(InFile.is_open())
  {
    while(getline(InFile, line))
    {
      cout << line << endl;
    }
  }

  cout << "End of the program." << endl;
  InFile.close();
  QueryFile.close();
  OutFile.close();
  return 0;
}


void File_Reader(string& line)
{

}


