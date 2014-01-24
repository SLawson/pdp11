//******************************************************************************
// initialize.h
//
//  Created on: Jan 20, 2014
//      Author: Rob Gaskell		PSU ID# 914464617
// 	   Project: pdp11
//******************************************************************************

#ifndef INITIALIZE_H_
#define INITIALIZE_H_

#include "pdp11.h"

//Function prototypes
int initialize(int RAM [], int argc, char * argv[], int PC_Reg);      //Reads ASCII, stores code/data sections in memory before execution begins

#endif//INITIALIZE_H_
