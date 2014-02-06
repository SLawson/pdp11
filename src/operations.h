//******************************************************************************
// operations.h
//
//  Created on: Jan 20, 2014
//      Author: Rob Gaskell		PSU ID# 914464617
// 	   Project: pdp11
//******************************************************************************

#ifndef OPERATIONS_H_
#define OPERATIONS_H_
#include "pdp11.h"
#include <stdint.h>

//Function prototypes
int Operation(int RAM[],instruction & current_inst, int GPR [], PSW & Status_word);        //Perform operation on current_inst operands
void StatusFlags(PSW & Status_word,int regDest, int ignore);
int AddressmodesDecode(int RAM[],instruction & current_inst, int GPR[]);
#endif//OPERATIONS_H_
