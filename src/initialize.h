//******************************************************************************
// initialize.h
//
//  Created on: Jan 20, 2014
//      Author: Rob Gaskell		PSU ID# 914464617
// 	   Project: pdp11
//******************************************************************************

#ifndef INITIALIZE_H_
#define INITIALIZE_H_

#include <string>
#include <stdlib.h>
#include <fstream>
#include "pdp11.h"

// structure for line parsing
struct command{
  char action;
  int value;
};

//Function prototypes
int initialize(int RAM [], int argc, char * argv[], int* PC_Reg);      //Reads ASCII, stores code/data sections in memory before execution begins
command parse_line(string line_in);


#endif//INITIALIZE_H_
