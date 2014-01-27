//******************************************************************************
// initialize.h
//
//  Created on: Jan 20, 2014
//      Author: Scott Lawson
// 	   Project: pdp11
//******************************************************************************

#ifndef INITIALIZE_H_
#define INITIALIZE_H_

#include <string>
#include <stdlib.h>
#include <fstream>
#include "pdp11.h"
#include "testing.h"

// structure for line parsing
struct command{
  char action;
  int value;
};

//Function prototypes
int initialize(int RAM [], int argc, char * argv[], int* PC_Reg, string& out_file);      //Reads ASCII, stores code/data sections in memory before execution begins
command parse_line(string line_in);         // parses a line from the input file into a command and value


#endif//INITIALIZE_H_

