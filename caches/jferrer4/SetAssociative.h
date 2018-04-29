#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include "string"
#include "instruction.h"

using namespace std;

class SetAssociative{
private:
  //Variables
  int cacheSize;
  int numEntries; //number of entries in the cache
  int numSetsInCache; //aka the number of lines
  int byteOffset;
  int indexSize;
  int set;

  int hits;
  int accesses;
  //counters for No Alloc on Write Miss
  int hitsNoAlloc;
  int accessesNoAlloc;
  //counters for Next Line Prefetching
  int hitsPref;
  int accessesPref;
  //counters for Prefetching on miss
  int hitsOnMiss;
  int accessesOnMiss;
  //counters for hot Cold
  int hitsHC;
  int accessesHC;


  //each line will be another vector
  //normal cache
  vector<vector<instruction> > cache;
  //no alloc on write miss cache
  vector<vector<instruction> > cacheNoAlloc;
  //next line prefetching cache
  vector<vector<instruction> > cachePref;
  //next line prefetching on miss cache
  vector<vector<instruction> > cacheOnMiss;
  //hot cold cache
  vector<instruction> cacheHC;


public:
  //constructor
  SetAssociative(int set);

  //ignore the load store
  void access(instruction inst, int accessTime);
  void accessNoAllocOnWrMiss(instruction inst, int accessTime);
  void accessPref(instruction inst, int accessTime);
  void accessOnMiss(instruction inst, int accessTime);
  void accessHotCold(instruction inst, int accessTime);

  //getters needed
  int getHits();
  int getAccesses();

  //getters for No alloc
  int getHitsNoAlloc();
  int getAccessesNoAlloc();

  //getters for Next Line Pref
  int getHitsPref();
  int getAccessesPref();

  //getters for Pref on miss
  int getHitsOnMiss();
  int getAccessesOnMiss();

  //getters for Hot Cold
  int getHitsHC();
  int getAccessesHC();

  int getNumEntries();

};
