#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <math.h> //log()
#include "string"

#include "DirectMapped.h"

using namespace std;

DirectMapped::DirectMapped(int cacheSize){
  this->cacheSize = cacheSize;

  //number of lines in the cache
  numLines = cacheSize / CACHE_LINE_SIZE;

  //byte offset: Since we only use  it will be 5 because 2^5 = 32
  //index: depends on the size of the cache
  byteOffset = log2(CACHE_LINE_SIZE);
  indexSize = log2(numLines);
  hits = 0;
  accesses = 0;

  //initialize the cache and zero out entries
  //using zero could give me a mistake since 0 is a valid tag
  //9223372036854775807 is the largest possible value, impossible for our cache
  unsigned long long temp = 9223372036854775807;
  for(int i = 0; i < numLines; i++){
    cache.push_back(temp);
  }
  //printf("%lu\n", cache.size());

}

void DirectMapped::access(unsigned long long address){
  //we need to take out the bits from the offset and from the index
  unsigned long long lineAddress = address >> byteOffset;
  unsigned long long tag = lineAddress >> indexSize;

  //(Block address) modulo (#Blocks in Cache)
  int cacheIndex = lineAddress % numLines;

  //if the cache at that location contains the tag calculated
  if(cache.at(cacheIndex) == tag){ //hit
    hits++;
    accesses++;
  } else{ //miss
    //replace the contents of the cache with the new tag
    cache.at(cacheIndex) = tag;
    accesses++;
  }
}

//getters
int DirectMapped::getHits(){
  return hits;
}

int DirectMapped::getAccesses(){
  return accesses;
}
