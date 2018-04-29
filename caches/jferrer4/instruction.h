#include <stdio.h>
#include <stdlib.h>
#ifndef INSTRUCTION
#define INSTRUCTION

#define KB_1 1024
#define KB_4 4*KB_1
#define KB_16 16*KB_1
#define KB_32 32*KB_1
#define CACHE_LINE_SIZE 32

//instruction structure to contain the type of instruction and the address
//this file also contains the the definitions for the cache sizes
struct instruction{
  char type;
  unsigned long long address;
  int tag;
  int lastAccessed;
  int leftOrRight;

};

#endif
