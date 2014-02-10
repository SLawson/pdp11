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

//Fetches an instruction, decodes and updates current_inst
void Fetch_Decode(int RAM [], int GPR [], instruction & current_inst, ofstream & file, bool I_or_D, PSW & Status_word);

//Perform a data fetch  
int Read_mem(int RAM [], int GPR [], ofstream&, bool I_or_D); 

//Perform memory write if required
int Write_mem(int RAM [], int & result, int & dest_addr, ofstream&);                 

#endif//MEMORY_H_
