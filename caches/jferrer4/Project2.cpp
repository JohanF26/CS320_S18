#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include "string"

//files
#include "instruction.h"
#include "DirectMapped.h"
#include "SetAssociative.h"


using namespace std;

int main(int argc, char* argv[]){
  //get name from command line and start a file stream
  ///argv[0]: Program name
  //argv[1]: input file
  //argv[2]: output file
  ifstream inputFile(argv[1]);
  ofstream outputFile(argv[2]);

  string currentLine;
  instruction currInst;
  vector<instruction> instructions;

  // Store all the instructions into a vector
  // I would have executed the instructions as they came in but due to
  //prefetching it would get messy as it would require me to get next line
  if(inputFile.is_open()){
    while(getline(inputFile, currentLine)){
      istringstream inputStream(currentLine);
      //store the type of instruction and read the address as hex and store it
      inputStream >> currInst.type >> hex >> currInst.address;
      instructions.push_back(currInst);
    }
    inputFile.close();
  }

  //Execute the different cache replacement policies after this

  //Direct Mapped Cache | models: 1KB, 4KB, 16KB, 32KB
  DirectMapped* DMC1 = new DirectMapped(KB_1);
  DirectMapped* DMC4 = new DirectMapped(KB_4);
  DirectMapped* DMC16 = new DirectMapped(KB_16);
  DirectMapped* DMC32 = new DirectMapped(KB_32);

  //Set Associative Cache | model: 16KB | set associativity: 2, 4, 8, 16
  SetAssociative* SAC2 = new SetAssociative(2);
  SetAssociative* SAC4 = new SetAssociative(4);
  SetAssociative* SAC8 = new SetAssociative(8);
  SetAssociative* SAC16 = new SetAssociative(16);

  //Fully Associative Cache | model: 16KB
  int magicNum = SAC2->getNumEntries(); //number to turn SAC into FAC
  SetAssociative* FAC_LRU = new SetAssociative(magicNum);

  //Set Associative Cache No-Allocation on Write Miss | model: 16KB | sets: 2, 4, 8, 16
  //No need to create new object, having a second cache saves us space
  //since it allows us to run the function necessary on a different cache

  //Set Associative Next-Line Prefetching
  //No need to create new object, having a third cache saves us space
  //since it allows us to run the function necessary on a different cache

  //Set Associative Next-Line Prefetching on Miss
  //No need to create new object, having a fourth cache saves us space
  //since it allows us to run the function necessary on a different cache

  for(int i = 0; i < instructions.size(); i++){
    //address of the instruction at current instruction index

    //Direct Mapped Cache
    DMC1->access(instructions.at(i).address);
    DMC4->access(instructions.at(i).address);
    DMC16->access(instructions.at(i).address);
    DMC32->access(instructions.at(i).address);

    //Set Associative Cache
    SAC2->access(instructions.at(i), i);
    SAC4->access(instructions.at(i), i);
    SAC8->access(instructions.at(i), i);
    SAC16->access(instructions.at(i), i);

    //Fully Associative Cache
    FAC_LRU->access(instructions.at(i), i);
    FAC_LRU->accessHotCold(instructions.at(i), i);

    //Set Associative Cache No-Allocation on Write Miss
    SAC2->accessNoAllocOnWrMiss(instructions.at(i), i);
    SAC4->accessNoAllocOnWrMiss(instructions.at(i), i);
    SAC8->accessNoAllocOnWrMiss(instructions.at(i), i);
    SAC16->accessNoAllocOnWrMiss(instructions.at(i), i);

    //Set Associative Cache Next-Line Prefetching
    SAC2->accessPref(instructions.at(i), 2*i);
    SAC4->accessPref(instructions.at(i), 2*i);
    SAC8->accessPref(instructions.at(i), 2*i);
    SAC16->accessPref(instructions.at(i), 2*i);

    //Set Associative Cache Prefetch on Miss
    SAC2->accessOnMiss(instructions.at(i), 2*i);
    SAC4->accessOnMiss(instructions.at(i), 2*i);
    SAC8->accessOnMiss(instructions.at(i), 2*i);
    SAC16->accessOnMiss(instructions.at(i), 2*i);

    //Fully Associative Cache Next Line Prefetching
    //FAC_LRU->accessPref(instructions.at(i), 2*i);

  }

  // printf("Direct Mapped Cache:\nHits: %d Accesses: %d\n", DMC1->getHits(), DMC1->getAccesses());
  // printf("Hits: %d Accesses: %d\n", DMC4->getHits(), DMC4->getAccesses());
  // printf("Hits: %d Accesses: %d\n", DMC16->getHits(), DMC16->getAccesses());
  // printf("Hits: %d Accesses: %d\n", DMC32->getHits(), DMC32->getAccesses());
  //
  // printf("Set Associative Cache:\nHits: %d Accesses: %d\n", SAC2->getHits(), SAC2->getAccesses());
  // printf("Hits: %d Accesses: %d\n", SAC4->getHits(), SAC4->getAccesses());
  // printf("Hits: %d Accesses: %d\n", SAC8->getHits(), SAC8->getAccesses());
  // printf("Hits: %d Accesses: %d\n", SAC16->getHits(), SAC16->getAccesses());
  //
  // printf("Fully Associative LRU:\nHits: %d Accesses: %d\n", FAC_LRU->getHits(), FAC_LRU->getAccesses());
  // printf("Fully Associative Hot Cold:\nHits: %d Accesses: %d\n", FAC_LRU->getHitsHC(), FAC_LRU->getAccessesHC());
  //
  // printf("Set Associative No-Alloc on Write Miss:\nHits: %d Accesses: %d\n", SAC2->getHitsNoAlloc(), SAC2->getAccessesNoAlloc());
  // printf("Hits: %d Accesses: %d\n", SAC4->getHitsNoAlloc(), SAC4->getAccessesNoAlloc());
  // printf("Hits: %d Accesses: %d\n", SAC8->getHitsNoAlloc(), SAC8->getAccessesNoAlloc());
  // printf("Hits: %d Accesses: %d\n", SAC16->getHitsNoAlloc(), SAC16->getAccessesNoAlloc());
  //
  // printf("Set Associative Next Line:\nHits: %d Accesses: %d\n", SAC2->getHitsPref(), SAC2->getAccessesPref());
  // printf("Hits: %d Accesses: %d\n", SAC4->getHitsPref(), SAC4->getAccessesPref());
  // printf("Hits: %d Accesses: %d\n", SAC8->getHitsPref(), SAC8->getAccessesPref());
  // printf("Hits: %d Accesses: %d\n", SAC16->getHitsPref(), SAC16->getAccessesPref());
  //
  // printf("Set Associative Next Line On Miss:\nHits: %d Accesses: %d\n", SAC2->getHitsOnMiss(), SAC2->getAccessesOnMiss());
  // printf("Hits: %d Accesses: %d\n", SAC4->getHitsOnMiss(), SAC4->getAccessesOnMiss());
  // printf("Hits: %d Accesses: %d\n", SAC8->getHitsOnMiss(), SAC8->getAccessesOnMiss());
  // printf("Hits: %d Accesses: %d\n", SAC16->getHitsOnMiss(), SAC16->getAccessesOnMiss());

  //printf("Extra-Credit: Fully Associative LRU Next Line:\nHits: %d Accesses: %d\n", FAC_LRU->getHitsPref(), FAC_LRU->getAccessesPref());

  ///___________________

  outputFile << DMC1->getHits() << "," << DMC1->getAccesses() << "; ";
  outputFile << DMC4->getHits() << "," << DMC4->getAccesses() << "; ";
  outputFile << DMC16->getHits() << "," << DMC16->getAccesses() << "; ";
  outputFile << DMC32->getHits() << "," << DMC32->getAccesses() << ";" << endl;

  outputFile << SAC2->getHits() << "," << SAC2->getAccesses() << "; ";
  outputFile << SAC4->getHits() << "," << SAC4->getAccesses() << "; ";
  outputFile << SAC8->getHits() << "," << SAC8->getAccesses() << "; ";
  outputFile << SAC16->getHits() << "," << SAC16->getAccesses() << ";" << endl;

  outputFile << FAC_LRU->getHits() << "," << FAC_LRU->getAccesses() << ";" << endl;
  outputFile << FAC_LRU->getHitsHC() << "," << FAC_LRU->getAccessesHC() << ";" << endl;

  outputFile << SAC2->getHitsNoAlloc() << "," << SAC2->getAccessesNoAlloc() << "; ";
  outputFile << SAC4->getHitsNoAlloc() << "," << SAC4->getAccessesNoAlloc() << "; ";
  outputFile << SAC8->getHitsNoAlloc() << "," << SAC8->getAccessesNoAlloc() << "; ";
  outputFile << SAC16->getHitsNoAlloc() << "," << SAC16->getAccessesNoAlloc() << ";" << endl;

  outputFile << SAC2->getHitsPref() << "," << SAC2->getAccessesPref() << "; ";
  outputFile << SAC4->getHitsPref() << "," << SAC4->getAccessesPref() << "; ";
  outputFile << SAC8->getHitsPref() << "," << SAC8->getAccessesPref() << "; ";
  outputFile << SAC16->getHitsPref() << "," << SAC16->getAccessesPref() << ";" << endl;

  outputFile << SAC2->getHitsOnMiss() << "," << SAC2->getAccessesOnMiss() << "; ";
  outputFile << SAC4->getHitsOnMiss() << "," << SAC4->getAccessesOnMiss() << "; ";
  outputFile << SAC8->getHitsOnMiss() << "," << SAC8->getAccessesOnMiss() << "; ";
  outputFile << SAC16->getHitsOnMiss() << "," << SAC16->getAccessesOnMiss() << ";" << endl;

  //printf("Fully Associative LRU Next Line:\nHits: %d Accesses: %d\n", FAC_LRU->getHitsPref(), FAC_LRU->getAccessesPref());
  outputFile.close();

  return 0;
}
