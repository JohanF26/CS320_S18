#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include "string"
#include "instruction.h"

using namespace std;

class DirectMapped{
private:
  //Variables will mostly be private
  int cacheSize; //data cache size
  int numLines; //number of lines in the cache
  int byteOffset; //how many bits does the byte offset take
  int indexSize; //how many bits does the index take

  int hits;
  int accesses; //total number of memory accesses
  vector<unsigned long long> cache;

public:
  //Functions will mostly be public

  //constructor
  DirectMapped(int cacheSize);

  //does not matter if address is load or store so we ignore the L/S Flag
  void access(unsigned long long address);

  //getters needed
  int getHits();
  int getAccesses();
};
