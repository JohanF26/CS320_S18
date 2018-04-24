#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "string"
#include "Predictors.h"
//include math to be able to do power since ^ is XOR
#include <math.h>
//pow is a double and takes doubles as input
//double pow (double base, double exponent);

using namespace std;

Predictors::Predictors(ifstream& input)
    : inputFile(input)
{

  pc = 0;
  properBehavior = "";

  //INITIALIZING COUNTERS TO 0
  totalInstructions = 0;
  correctTaken = 0;
  correctNotTaken = 0;

  for(int i = 0; i < 9 ; i++){
    if(i < 7){
      //these arrays are of size 7
      correctSingleBC[i] = 0;
      correctTwoBC[i] = 0;
    }
    //array of size 9
    correctGS[i] = 0;

    //GLOBAL HISTORY REGISTERS
    globalGS[i] = 0;
  }
  globalTournament = N;
  ////////////////////////////

  correctTournament = 0;

  //INITIALIZING TABLES ENTRIES TO TT or T
  for(int i = 0; i < 16; i++){
    singleBT16[i] = T;
    twoBT16[i] = TT;
  }

  for(int i = 0; i < 32; i++){
    singleBT32[i] = T;
    twoBT32[i] = TT;
  }

  for(int i = 0; i < 128; i++){
    singleBT128[i] = T;
    twoBT128[i] = TT;
  }

  for(int i = 0; i < 256; i++){
    singleBT256[i] = T;
    twoBT256[i] = TT;
  }

  for(int i = 0; i < 512; i++){
    singleBT512[i] = T;
    twoBT512[i] = TT;
  }

  for(int i = 0; i < 1024; i++){
    singleBT1024[i] = T;
    twoBT1024[i] = TT;
  }

  for(int i = 0; i < 2048; i++){
    singleBT2048[i] = T;
    twoBT2048[i] = TT;
    tournamentTBT[i] = TT;
    tournamentGST[i] = TT;
    selectorT[i] = GG;

    //gShare Table entries
    for(int j = 0; j < 9; j++){
      gshareT[j][i] = TT;
    }
  }
}

void Predictors::BranchPredictors(){
  string currentLine;

  //continue getting lines until end of file
  while(getline(inputFile, currentLine)){
    //count the amount of instructions
    totalInstructions++;

    //to figure out length of memory address
    int spaceIndex;
    for(spaceIndex = 0; spaceIndex < currentLine.length(); spaceIndex++){
      if(currentLine.at(spaceIndex) == ' '){
        break;
      }
    }
    //slice the string to get pc and behavior
    int sz = 0;
    istringstream inputStream(currentLine);
    //string stringPC = currentLine.substr(0, spaceIndex);
    //properBehavior = currentLine.substr(spaceIndex+1);
    inputStream >> hex >> pc >> properBehavior;

    int mostSigBitOne;
    int leastSigBits;
    int zeroLeastSigBits;
    //position of counter for current table
    int counterPos;

    int gsIndex;
    int bits;
    int tournamentIndex = (globalTournament ^ pc) % ((int) pow(2,11));
    int selectorIndex = pc % ((int) pow(2,11));

    bool gsIsCorrect = false;
    bool btIsCorrect = false;

    //2^11 = 2048
    if(properBehavior == "T"){
      //always taken
      correctTaken++;

      for(int i = 4; i < 12; i++){
        zeroLeastSigBits = (int)pow(2,i);
        counterPos = i-4;
        //get least sig bits to index into table
        leastSigBits = pc%zeroLeastSigBits;

        if(i == 4){

          if(singleBT16[leastSigBits] == T){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT16[leastSigBits] = T;
          }

          if(twoBT16[leastSigBits] == TT){
            correctTwoBC[counterPos]++;
          } else if(twoBT16[leastSigBits] == T){
            correctTwoBC[counterPos]++;
            twoBT16[leastSigBits] = TT;
          } else if(twoBT16[leastSigBits] == N){
            twoBT16[leastSigBits] = T;
          } else {
            twoBT16[leastSigBits] = N;
          }

        }

        if(i == 5){
          if(singleBT32[leastSigBits] == T){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT32[leastSigBits] = T;
          }

          if(twoBT32[leastSigBits] == TT){
            correctTwoBC[counterPos]++;
          } else if(twoBT32[leastSigBits] == T){
            correctTwoBC[counterPos]++;
            twoBT32[leastSigBits] = TT;
          } else if(twoBT32[leastSigBits] == N){
            twoBT32[leastSigBits] = T;
          } else {
            twoBT32[leastSigBits] = N;
          }
        }

        if(i == 7){
          counterPos--;
          if(singleBT128[leastSigBits] == T){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT128[leastSigBits] = T;
          }

          if(twoBT128[leastSigBits] == TT){
            correctTwoBC[counterPos]++;
          } else if(twoBT128[leastSigBits] == T){
            correctTwoBC[counterPos]++;
            twoBT128[leastSigBits] = TT;
          } else if(twoBT128[leastSigBits] == N){
            twoBT128[leastSigBits] = T;
          } else {
            twoBT128[leastSigBits] = N;
          }
        }

        if(i == 8){
          counterPos--;
          if(singleBT256[leastSigBits] == T){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT256[leastSigBits] = T;
          }

          if(twoBT256[leastSigBits] == TT){
            correctTwoBC[counterPos]++;
          } else if(twoBT256[leastSigBits] == T){
            correctTwoBC[counterPos]++;
            twoBT256[leastSigBits] = TT;
          } else if(twoBT256[leastSigBits] == N){
            twoBT256[leastSigBits] = T;
          } else {
            twoBT256[leastSigBits] = N;
          }
        }

        if(i == 9){
          counterPos--;
          if(singleBT512[leastSigBits] == T){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT512[leastSigBits] = T;
          }

          if(twoBT512[leastSigBits] == TT){
            correctTwoBC[counterPos]++;
          } else if(twoBT512[leastSigBits] == T){
            correctTwoBC[counterPos]++;
            twoBT512[leastSigBits] = TT;
          } else if(twoBT512[leastSigBits] == N){
            twoBT512[leastSigBits] = T;
          } else {
            twoBT512[leastSigBits] = N;
          }
        }

        if(i == 10){
          counterPos--;
          if(singleBT1024[leastSigBits] == T){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT1024[leastSigBits] = T;
          }

          if(twoBT1024[leastSigBits] == TT){
            correctTwoBC[counterPos]++;
          } else if(twoBT1024[leastSigBits] == T){
            correctTwoBC[counterPos]++;
            twoBT1024[leastSigBits] = TT;
          } else if(twoBT1024[leastSigBits] == N){
            twoBT1024[leastSigBits] = T;
          } else {
            twoBT1024[leastSigBits] = N;
          }
        }

        if(i == 11){
          counterPos--;
          if(singleBT2048[leastSigBits] == T){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT2048[leastSigBits] = T;
          }

          if(twoBT2048[leastSigBits] == TT){
            correctTwoBC[counterPos]++;
          } else if(twoBT2048[leastSigBits] == T){
            correctTwoBC[counterPos]++;
            twoBT2048[leastSigBits] = TT;
          } else if(twoBT2048[leastSigBits] == N){
            twoBT2048[leastSigBits] = T;
          } else {
            twoBT2048[leastSigBits] = N;
          }

          //gshare
          for(int j = 0; j < 9; j++){
            bits = i - 8 + j;
            //only XOR Least significant bits
            //gsIndex = ((globalGS[j] % ((int) pow(2,bits))) ^ pc) % ((int) pow(2,11)); //XOR
            gsIndex = (globalGS[j] & ((int) pow(2,bits) - 1)) ^ (pc%2048);

            //j+3 bit
            if(gshareT[j][gsIndex] == TT){
              correctGS[j]++;
            } else if(gshareT[j][gsIndex] == T){
              correctGS[j]++;
              gshareT[j][gsIndex] = TT;
            } else if(gshareT[j][gsIndex] == N){
              gshareT[j][gsIndex] = T;
            } else{
              gshareT[j][gsIndex] = N;
            }
            //shift left and add most recent
            globalGS[j] <<= 1;
            globalGS[j]++;
          }

          //tournament
          //since it is taken we need to update global history register
          globalTournament <<= 1;
          globalTournament++;

          if(tournamentTBT[selectorIndex] == TT || tournamentTBT[selectorIndex] == T){
            btIsCorrect = true;
            tournamentTBT[selectorIndex] = TT;
            if(selectorT[selectorIndex] == BB || selectorT[selectorIndex] == B){
              correctTournament++;
            }
          } else {
            btIsCorrect = false;
            tournamentTBT[selectorIndex]++;
          }

          if(tournamentGST[tournamentIndex] == TT || tournamentGST[tournamentIndex] == T){
            gsIsCorrect = true;
            tournamentGST[tournamentIndex] = TT;
            if(selectorT[selectorIndex] == GG || selectorT[selectorIndex] == G){
              correctTournament++;
            }
          } else {
            gsIsCorrect = false;
            tournamentGST[tournamentIndex]++;
          }

          //update selector table
          if(btIsCorrect == true && gsIsCorrect == false){
            if(selectorT[selectorIndex] != BB){
              selectorT[selectorIndex]++;
            }
          } else if(btIsCorrect == false && gsIsCorrect == true){
            if(selectorT[selectorIndex] != GG){
              selectorT[selectorIndex]--;
            }
          }
        }
      }

    } else{
      //always not taken
      correctNotTaken++;

      for(int i = 4; i < 12; i++){
        zeroLeastSigBits = (int)pow(2,i);
        counterPos = i-4;
        //get least sig bits to index into table
        leastSigBits = pc%zeroLeastSigBits;

        //Bimodal Single Bit Predictor
        if(i == 4){
          if(singleBT16[leastSigBits] == N){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT16[leastSigBits] = N;
          }

          if(twoBT16[leastSigBits] == NN){
            correctTwoBC[counterPos]++;
          } else if(twoBT16[leastSigBits] == N){
            correctTwoBC[counterPos]++;
            twoBT16[leastSigBits] = NN;
          } else if(twoBT16[leastSigBits] == T){
            twoBT16[leastSigBits] = N;
          } else {
            twoBT16[leastSigBits] = T;
          }
        }

        if(i == 5){
          if(singleBT32[leastSigBits] == N){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT32[leastSigBits] = N;
          }

          if(twoBT32[leastSigBits] == NN){
            correctTwoBC[counterPos]++;
          } else if(twoBT32[leastSigBits] == N){
            correctTwoBC[counterPos]++;
            twoBT32[leastSigBits] = NN;
          } else if(twoBT32[leastSigBits] == T){
            twoBT32[leastSigBits] = N;
          } else {
            twoBT32[leastSigBits] = T;
          }
        }

        if(i == 7){
          counterPos--;
          if(singleBT128[leastSigBits] == N){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT128[leastSigBits] = N;
          }

          if(twoBT128[leastSigBits] == NN){
            correctTwoBC[counterPos]++;
          } else if(twoBT128[leastSigBits] == N){
            correctTwoBC[counterPos]++;
            twoBT128[leastSigBits] = NN;
          } else if(twoBT128[leastSigBits] == T){
            twoBT128[leastSigBits] = N;
          } else {
            twoBT128[leastSigBits] = T;
          }
        }

        if(i == 8){
          counterPos--;
          if(singleBT256[leastSigBits] == N){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT256[leastSigBits] = N;
          }

          if(twoBT256[leastSigBits] == NN){
            correctTwoBC[counterPos]++;
          } else if(twoBT256[leastSigBits] == N){
            correctTwoBC[counterPos]++;
            twoBT256[leastSigBits] = NN;
          } else if(twoBT256[leastSigBits] == T){
            twoBT256[leastSigBits] = N;
          } else {
            twoBT256[leastSigBits] = T;
          }
        }

        if(i == 9){
          counterPos--;
          if(singleBT512[leastSigBits] == N){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT512[leastSigBits] = N;
          }

          if(twoBT512[leastSigBits] == NN){
            correctTwoBC[counterPos]++;
          } else if(twoBT512[leastSigBits] == N){
            correctTwoBC[counterPos]++;
            twoBT512[leastSigBits] = NN;
          } else if(twoBT512[leastSigBits] == T){
            twoBT512[leastSigBits] = N;
          } else {
            twoBT512[leastSigBits] = T;
          }
        }

        if(i == 10){
          counterPos--;
          if(singleBT1024[leastSigBits] == N){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT1024[leastSigBits] = N;
          }

          if(twoBT1024[leastSigBits] == NN){
            correctTwoBC[counterPos]++;
          } else if(twoBT1024[leastSigBits] == N){
            correctTwoBC[counterPos]++;
            twoBT1024[leastSigBits] = NN;
          } else if(twoBT1024[leastSigBits] == T){
            twoBT1024[leastSigBits] = N;
          } else {
            twoBT1024[leastSigBits] = T;
          }
        }

        if(i == 11){
          counterPos--;
          if(singleBT2048[leastSigBits] == N){
            correctSingleBC[counterPos]++;
          } else {
            //if incorrect flip bit at that index
            singleBT2048[leastSigBits] = N;
          }

          if(twoBT2048[leastSigBits] == NN){
            correctTwoBC[counterPos]++;
          } else if(twoBT2048[leastSigBits] == N){
            correctTwoBC[counterPos]++;
            twoBT2048[leastSigBits] = NN;
          } else if(twoBT2048[leastSigBits] == T){
            twoBT2048[leastSigBits] = N;
          } else {
            twoBT2048[leastSigBits] = T;
          }

          //gshare
          for(int j = 0; j < 9; j++){
            bits = i - 8 + j;
            //least significant bits
            //gsIndex = ((globalGS[j] % ((int) pow(2,bits))) ^ pc) % ((int) pow(2,11)); //XOR
            //gsIndex = ((pc%2048)^(globalGS[j] % ((int) pow(2,bits))));
            gsIndex = (globalGS[j] & ((int) pow(2,bits) - 1)) ^ (pc%2048);
            //j+3 bit
            if(gshareT[j][gsIndex] == NN){
              correctGS[j]++;
            } else if(gshareT[j][gsIndex] == N){
              correctGS[j]++;
              gshareT[j][gsIndex] = NN;
            } else if(gshareT[j][gsIndex] == T){
              gshareT[j][gsIndex] = N;
            } else{
              gshareT[j][gsIndex] = T;
            }
            //shift left and add most recent
            globalGS[j] <<= 1;
          }

          //tournament
          //since it is taken we need to update global history register
          globalTournament <<= 1;

          if(tournamentTBT[tournamentIndex] == NN || tournamentTBT[tournamentIndex] == N){
            btIsCorrect = true;
            tournamentTBT[tournamentIndex] = NN;
            if(selectorT[leastSigBits] == BB || selectorT[leastSigBits] == B){
              correctTournament++;
            }
          } else {
            btIsCorrect = false;
            //move towards not taken
            tournamentTBT[tournamentIndex]--;
          }

          if(tournamentGST[tournamentIndex] == NN || tournamentGST[tournamentIndex] == N){
            gsIsCorrect = true;
            tournamentGST[tournamentIndex] = NN;
            if(selectorT[leastSigBits] == GG || selectorT[leastSigBits] == G){
              correctTournament++;
            }
          } else {
            gsIsCorrect = false;
            //move towards not taken
            tournamentGST[tournamentIndex]--;
          }

          //update selector table
          if(btIsCorrect == true && gsIsCorrect == false){
            if(selectorT[leastSigBits] != BB){
              selectorT[leastSigBits]++;
            }
          } else if(btIsCorrect == false && gsIsCorrect == true){
            if(selectorT[leastSigBits] != GG){
              selectorT[leastSigBits]--;
            }
          }
        }
      }
    }
  }
}

int Predictors::getTotalInstructions(){
  return totalInstructions;
}

int Predictors::getCorrectTaken(){
  return correctTaken;
}

int Predictors::getCorrectNotTaken(){
  return correctNotTaken;
}

int* Predictors::getCorrectSingleBC(){
  return correctSingleBC;
}

int* Predictors::getCorrectTwoBC(){
  return correctTwoBC;
}

int* Predictors::getCorrectGS(){
  return correctGS;
}

int Predictors::getCorrectTournament(){
  return correctTournament;
}








//////
