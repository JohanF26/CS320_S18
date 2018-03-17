#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include "string"
//include math to be able to do power since ^ is XOR
#include <math.h>
//pow is a double and takes doubles as input
//double pow (double base, double exponent);

using namespace std;

class Predictors{
  private:
    //file to read
    ifstream& inputFile;

    //extracted from each line
    unsigned long long pc;
    string properBehavior;

    //enums for possible states and tournament preferences
    enum states{NN, N, T, TT};
    enum preference{GG, G, B, BB};

    //////////  CORRECT PREDICTIONS COUNTER  //////////

    //counter for total number of lines read
    int totalInstructions;

    //counter for always taken
    int correctTaken;
    //counter for always not taken
    int correctNotTaken;
    //bimodal single bit correct counter 0=16-bit 1=32-bits 2=128-bits etc
    int correctSingleBC[7];
    //bimodal 2-bit correct counter 0=16-bit 1=32-bits 2=128-bits etc
    int correctTwoBC[7];
    //gshare correct counter
    int correctGS[9];
    //tournament correct counters
    int correctTournament;

    //////////  BIMODAL TABLES  //////////

    //sinlge bit tables (arrays)
    int singleBT16[16];
    int singleBT32[32];
    int singleBT128[128];
    int singleBT256[256];
    int singleBT512[512];
    int singleBT1024[1024];
    int singleBT2048[2048];

    //two bit tables
    int twoBT16[16];
    int twoBT32[32];
    int twoBT128[128];
    int twoBT256[256];
    int twoBT512[512];
    int twoBT1024[1024];
    int twoBT2048[2048];

    /*
      Varying global history length implies creating a 9 different tables since
      each table contains only one global history register

      GShare tables 3-11 and since they are all the same size I could turn them
      into a two dimensional array of 9 different tables with 2048 entries
    */
    int gshareT[9][2048];

    //Tournament tables
    int tournamentTBT[2048];
    int tournamentGST[2048];
    int selectorT[2048];

    //////////  GLOBAL HISTORY REGISTERS //////////

    //gshare global history registers for each table
    int globalGS[9];

    //tournament global history register shared between gshare and bimodal 2-bit
    int globalTournament;

  public:
    Predictors(ifstream&);
    void BranchPredictors();

    //////////  CORRECT PREDICTIONS COUNTER GETTERS  //////////
    int getTotalInstructions();
    int getCorrectTaken();
    int getCorrectNotTaken();
    int* getCorrectSingleBC();
    int* getCorrectTwoBC();
    int* getCorrectGS();
    int getCorrectTournament();

};
