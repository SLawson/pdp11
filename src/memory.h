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
#include "testing.h"
#include <stdint.h>

//Fetches an instruction, decodes and updates current_inst
void Fetch_Decode(instruction & current_inst, bool I_or_D);

//Perform a data fetch
int access_mem(uint16_t index, int flag, int out=0);


#endif//MEMORY_H_
