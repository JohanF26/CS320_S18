#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include "string"
#include "Predictors.h"

using namespace std;

int main(int argc, char *argv[]){
  //start file stream with name from command line
  //argv[0] is the program name
  ifstream input(argv[1]);
  ofstream output(argv[2]);

  if(input.is_open()){
    //class constructor
    Predictors* newPredictors = new Predictors(input);
    //branch predictors
    newPredictors->BranchPredictors();

    int instructions = newPredictors->getTotalInstructions();
    int alwaysTaken = newPredictors->getCorrectTaken();
    int alwaysNotTaken = newPredictors->getCorrectNotTaken();
    int tournament = newPredictors->getCorrectTournament();

    output << alwaysTaken << "," << instructions << ";" << endl;
    output << alwaysNotTaken << "," << instructions << ";" << endl;

    //one-bit bimodal
    for(int i = 0; i < 7; i++){
      output << newPredictors->getCorrectSingleBC()[i] << "," << instructions << ";";
      if(i != 6){
        output << " ";
      }
    }
    output << endl;

    //two bit bimodal
    for(int i = 0; i < 7; i++){
      output << newPredictors->getCorrectTwoBC()[i] << "," << instructions << ";";
      if(i != 6){
        output << " ";
      }
    }
    output << endl;

    //gshare
    for(int i = 0; i < 9; i++){
      output << newPredictors->getCorrectGS()[i] << "," << instructions << ";";
      if(i != 8){
        output << " ";
      }
    }
    output << endl;

    //tournament
    output << tournament << "," << instructions << ";" << endl;

    //close files
    input.close();
    output.close();
  }

  return 0;
}
