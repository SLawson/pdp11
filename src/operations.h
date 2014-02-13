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
void Operation(instruction & current_inst, PSW & Status_word);        //Perform operation on current_inst operands
void StatusFlags(int regDest, int ignore);
int AddressmodesDecode(int mode, int &address_op,int curr_Register,int prog_cntr);
void TakeBranch(int & Program_Counter, int & branch_offset);

#endif//OPERATIONS_H_
