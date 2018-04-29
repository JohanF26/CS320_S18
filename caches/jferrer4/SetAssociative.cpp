#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <math.h> //log()
#include "string"

#include "SetAssociative.h"

using namespace std;

SetAssociative::SetAssociative(int set)
              :cacheSize(KB_16)
{
  this->set = set;
  //number of entries in the cache
  numEntries = cacheSize / CACHE_LINE_SIZE;
  //each line has "set" number of entries
  numSetsInCache = numEntries / set;

  //byte offset
  //index into table
  byteOffset = log2(CACHE_LINE_SIZE);
  indexSize = log2(numSetsInCache);

  hits = 0;
  accesses = 0;

  hitsNoAlloc = 0;
  accessesNoAlloc = 0;

  hitsPref = 0;
  accessesPref = 0;

  hitsOnMiss = 0;
  accessesOnMiss = 0;

  hitsHC = 0;
  accessesHC = 0;

  //initiate the cache and zero out entries
  instruction tempInst;
  tempInst.address = 9223372036854775807; //largest possible
  tempInst.lastAccessed = -1; //possible to access at time 0
  tempInst.leftOrRight = 0; //set to 1 when it is hot and 0 when it is cold
  vector<instruction> tempVect;

  instruction tempInst2;
  tempInst2.address = 9223372036854775807;
  tempInst2.lastAccessed = 2147483647;
  vector<instruction> tempVect2;
  for(int i = 0; i < set; i++){
    //create a sample cache set
    tempVect.push_back(tempInst);
    tempVect2.push_back(tempInst2);
    cacheHC.push_back(tempInst);
  }
  for(int i = 0; i < numSetsInCache; i++){
    //loops "set" times through the cache, accessing the next set each time
    cache.push_back(tempVect);
    //modify function to also initialize more caches
    cacheNoAlloc.push_back(tempVect);
    cachePref.push_back(tempVect);
    cacheOnMiss.push_back(tempVect);
  }
}

//uses LRU algorithm used in my CS350 prog4
void SetAssociative::access(instruction inst, int accessTime){
  //if we were to count the entries as in 4 entries per cache line
  instruction leastRecent; //least recent instruction
  int lrIndex; //index of the least recent address
  //bool h = false; //was there a hit or a miss

  inst.lastAccessed = accessTime;
  leastRecent.lastAccessed = accessTime;
  lrIndex = 0;
  unsigned long long blockNumber = inst.address >> byteOffset;
  inst.tag = blockNumber >> indexSize;

  //(Block Number) modulo (#Sets in cache)
  int setIndex = blockNumber % numSetsInCache;

  //look through the entries in the current set of entries
  for(int s = 0; s < set; s++){
    //cache.at(setIndex) returns a vector containing "set" instructions
    //cache.at(setIndex).at(s) returns the instruction at location s of the current set
    if(cache.at(setIndex).at(s).tag == inst.tag){ //hit
      hits++;
      accesses++;
      //h = true;
      //update access time
      cache.at(setIndex).at(s).lastAccessed = inst.lastAccessed;
      return; //nothing else to do here
    } else if(cache.at(setIndex).at(s).lastAccessed < leastRecent.lastAccessed){ //miss
      //if lastAccessed < leastRecent.lastAccessed then this was used least recently
      lrIndex = s;
      //redefine least recent as many times as necessary to find least recently used
      leastRecent = cache.at(setIndex).at(s);
    }
  }

  //if it gets past this then there was not a hit so replace lru with new instruction
  cache.at(setIndex).at(lrIndex) = inst;
  accesses++;
}

//use second cache
void SetAssociative::accessNoAllocOnWrMiss(instruction inst, int accessTime){
  //if we were to count the entries as in 4 entries per cache line
  instruction leastRecent; //least recent instruction
  int lrIndex; //index of the least recent address
  //bool h = false; //was there a hit or a miss

  inst.lastAccessed = accessTime;
  leastRecent.lastAccessed = accessTime;
  lrIndex = 0;
  unsigned long long blockNumber = inst.address >> byteOffset;
  inst.tag = blockNumber >> indexSize;

  //(Block Number) modulo (#Sets in cache)
  int setIndex = blockNumber % numSetsInCache;

  //look through the entries in the current set of entries
  for(int s = 0; s < set; s++){
    //cache.at(setIndex) returns a vector containing "set" instructions
    //cache.at(setIndex).at(s) returns the instruction at location s of the current set
    if(cacheNoAlloc.at(setIndex).at(s).tag == inst.tag){ //hit
      hitsNoAlloc++;
      accessesNoAlloc++;
      //h = true;
      //update access time
      cacheNoAlloc.at(setIndex).at(s).lastAccessed = inst.lastAccessed;
      return; //nothing else to do here
    } else if(cacheNoAlloc.at(setIndex).at(s).lastAccessed < leastRecent.lastAccessed){ //miss
      //if lastAccessed < leastRecent.lastAccessed then this was used least recently
      lrIndex = s;
      //redefine least recent as many times as necessary to find least recently used
      leastRecent = cacheNoAlloc.at(setIndex).at(s);
    }
  }
  //if it gets past this then there was not a hit so replace lru with new instruction

  //only if the instruction is NOT a store
  if(inst.type != 'S'){
    cacheNoAlloc.at(setIndex).at(lrIndex) = inst;
  }
  accessesNoAlloc++;
}

void SetAssociative::accessPref(instruction inst, int accessTime){
  //if we were to count the entries as in 4 entries per cache line
  instruction leastRecent; //least recent instruction
  instruction leastRecent2; //second least recent instruction
  int lrIndex; //index of the least recent address
  int lrIndex2; //index of the second least recent address
  bool h = false; //was there a hit or a miss
  bool p = false;

  instruction nextLine = inst;
  //access time passed *2 as to allow prefetching
  nextLine.address += 32;
  inst.lastAccessed = accessTime;
  nextLine.lastAccessed = accessTime+1;

  //next line prefetching

  leastRecent.lastAccessed = accessTime+1;
  leastRecent2.lastAccessed = accessTime;

  lrIndex = 0;
  lrIndex2 = 0;

  unsigned long long blockNumber = inst.address >> byteOffset;
  unsigned long long blockNumber2 = nextLine.address >> byteOffset;
  inst.tag = blockNumber >> indexSize;
  nextLine.tag = blockNumber2 >> indexSize;

  //(Block Number) modulo (#Sets in cache)
  int setIndex = blockNumber % numSetsInCache;
  int setIndex2 = blockNumber2 % numSetsInCache;

  //look through the entries in the current set of entries
  for(int s = 0; s < set; s++){
    //cache.at(setIndex) returns a vector containing "set" instructions
    //cache.at(setIndex).at(s) returns the instruction at location s of the current set
    if((cachePref.at(setIndex).at(s).tag == inst.tag) && !h){ //hit
      hitsPref++;
      accessesPref++;
      h = true;
      //update access time
      cachePref.at(setIndex).at(s).lastAccessed = inst.lastAccessed;
    } else if((cachePref.at(setIndex).at(s).lastAccessed < leastRecent.lastAccessed) && !h){ //miss
      //if lastAccessed < leastRecent.lastAccessed then this was used least recently
      lrIndex = s;
      //redefine least recent as many times as necessary to find least recently used
      leastRecent = cachePref.at(setIndex).at(s);
    }
  }
  for(int s = 0; s < set; s++){
    //next line Prefetching
    //if it is already in the cache dont do anything
    if((cachePref.at(setIndex2).at(s).tag == nextLine.tag) && !p){
      //set all fields equal to each other
      cachePref.at(setIndex2).at(s) = nextLine;
      p = true;
    } else if((cachePref.at(setIndex2).at(s).lastAccessed < leastRecent2.lastAccessed) && !p){
      lrIndex2 = s;
      leastRecent2 = cachePref.at(setIndex2).at(s);
    }
  }

  if(!h){
    cachePref.at(setIndex).at(lrIndex) = inst;
    accessesPref++;
  }
  if(!p){
    cachePref.at(setIndex2).at(lrIndex2) = nextLine;
  }

}

void SetAssociative::accessOnMiss(instruction inst, int accessTime){
  //if we were to count the entries as in 4 entries per cache line
  instruction leastRecent; //least recent instruction
  instruction leastRecent2; //second least recent instruction
  int lrIndex; //index of the least recent address
  int lrIndex2; //index of the second least recent address
  bool h = false; //was there a hit or a miss
  bool p = false;

  instruction nextLine = inst;
  //access time passed *2 as to allow prefetching
  nextLine.address += 32;
  inst.lastAccessed = accessTime;
  nextLine.lastAccessed = accessTime+1;

  //next line prefetching

  leastRecent.lastAccessed = accessTime+1;
  leastRecent2.lastAccessed = accessTime;

  lrIndex = 0;
  lrIndex2 = 0;

  unsigned long long blockNumber = inst.address >> byteOffset;
  unsigned long long blockNumber2 = nextLine.address >> byteOffset;
  inst.tag = blockNumber >> indexSize;
  nextLine.tag = blockNumber2 >> indexSize;

  //(Block Number) modulo (#Sets in cache)
  int setIndex = blockNumber % numSetsInCache;
  int setIndex2 = blockNumber2 % numSetsInCache;

  //look through the entries in the current set of entries
  for(int s = 0; s < set; s++){
    //cache.at(setIndex) returns a vector containing "set" instructions
    //cache.at(setIndex).at(s) returns the instruction at location s of the current set
    if((cacheOnMiss.at(setIndex).at(s).tag == inst.tag) && !h){ //hit
      hitsOnMiss++;
      accessesOnMiss++;
      h = true;
      //update access time
      cacheOnMiss.at(setIndex).at(s).lastAccessed = inst.lastAccessed;
    } else if((cacheOnMiss.at(setIndex).at(s).lastAccessed < leastRecent.lastAccessed) && !h){ //miss
      //if lastAccessed < leastRecent.lastAccessed then this was used least recently
      lrIndex = s;
      //redefine least recent as many times as necessary to find least recently used
      leastRecent = cacheOnMiss.at(setIndex).at(s);
    }
  }
  for(int s = 0; s < set; s++){
    //next line Prefetching
    //if it is already in the cache dont do anything
    if((cacheOnMiss.at(setIndex2).at(s).tag == nextLine.tag) && !p && !h){
      //set all fields equal to each other
      cacheOnMiss.at(setIndex2).at(s) = nextLine;
      p = true;
    } else if((cacheOnMiss.at(setIndex2).at(s).lastAccessed < leastRecent2.lastAccessed) && !p){
      lrIndex2 = s;
      leastRecent2 = cacheOnMiss.at(setIndex2).at(s);
    }
  }

  if(!h){
    cacheOnMiss.at(setIndex).at(lrIndex) = inst;
    accessesOnMiss++;
    if(!p){
      cacheOnMiss.at(setIndex2).at(lrIndex2) = nextLine;
    }
  }
}

void SetAssociative::accessHotCold(instruction inst, int accessTime){
  //to determine if the current node came from the left or the right
  int left;
  int right;
  int nodeIndex;
  int victimIndex;
  int tempIndex;
  int h = 0;

  inst.lastAccessed = accessTime;

  unsigned long long blockNumber = inst.address >> byteOffset;
  inst.tag = blockNumber >> indexSize;

  //look through the cache
  //printf("1\n");
  for(int i = 0; i < cacheHC.size(); i++){
    //if we found the set with the tag its a hit
    //printf("2\n");
    if(cacheHC.at(i).tag == inst.tag){
      //find the cache
      hitsHC++;
      accessesHC++;
      cacheHC.at(i).lastAccessed = inst.lastAccessed;
      nodeIndex = i;
      //while loop to look back through the cache and update hot or cold
      while(nodeIndex != 0){
        left = (nodeIndex-1)/2; //parent node if current is hot
        right = (nodeIndex-2)/2; //parent node if current is cold
        if(left == 0){
          cacheHC.at(left).leftOrRight = 0; //left child is hot
        } else if(left == right){
          cacheHC.at(left).leftOrRight = 1; //right child is hot
        } else{
          cacheHC.at(left).leftOrRight = 0;
        }
        nodeIndex = left;
      }
      //printf("3\n");
      return; //if there was a hit there is nothing to do here
    }
    //if(h){ //if there was a hit
    //  return;
    //}
  }
  //printf("4\n");
  //now look for a victim to replace only when there has not been a hit
  for(victimIndex = 0; victimIndex < cacheHC.size(); victimIndex = victimIndex){
    tempIndex = ((2*victimIndex) + 1); //set it to its right child
    //if left child hot add 1 to the victimIndex
    //printf("5\n");
    tempIndex = tempIndex + (cacheHC.at(victimIndex).leftOrRight + 1) % 2;
    //printf("6\n");
    if(tempIndex < cacheHC.size()){
      victimIndex = tempIndex;
    } else{
      break;
    }
  }

  //replace node at victimIndex
  //printf("6.5\n");
  cacheHC.at(victimIndex) = inst;
  //printf("7\n");
  //update the cache with hot or cold
  while(victimIndex != 0){
    left = (victimIndex-1)/2;
    right = (victimIndex-1)/2;
    if(left == 0){
      cacheHC.at(left).leftOrRight = 0; //left child is hot
    } else if(left == right){
      cacheHC.at(left).leftOrRight = 1; //right child is hot
    } else{
      cacheHC.at(left).leftOrRight = 0;
    }
    victimIndex = left;
  }
  accessesHC++;

}

//getters
int SetAssociative::getHits(){
  return hits;
}

int SetAssociative::getAccesses(){
  return accesses;
}

int SetAssociative::getHitsNoAlloc(){
  return hitsNoAlloc;
}

int SetAssociative::getAccessesNoAlloc(){
  return accessesNoAlloc;
}

int SetAssociative::getHitsPref(){
  return hitsPref;
}

int SetAssociative::getAccessesPref(){
  return accessesPref;
}

int SetAssociative::getHitsOnMiss(){
  return hitsOnMiss;
}

int SetAssociative::getAccessesOnMiss(){
  return accessesOnMiss;
}

int SetAssociative::getHitsHC(){
  return hitsHC;
}

int SetAssociative::getAccessesHC(){
  return accessesHC;
}

int SetAssociative::getNumEntries(){
  return numEntries;
}
