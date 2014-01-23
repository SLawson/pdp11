//******************************************************************************
// memory.h
//
//  Created on: Jan 20, 2014
//      Author: Rob Gaskell
// 	   Project: pdp11
//******************************************************************************

#ifndef MEMORY_H_
#define MEMORY_H_

#include "pdp11.h"

int Fetch_Decode(int RAM [], instruction & current_inst);     //Fetches an instruction, decodes and updates current_inst
int Write_mem(int & result, int & dest_addr);                 //Perform memory write if required

#endif//MEMORY_H_
