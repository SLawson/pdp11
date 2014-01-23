//******************************************************************************
// memory.cpp
//
//  Created on: Jan 20, 2014
//      Author: Rob Gaskell
// 	   Project: pdp11
//******************************************************************************

#include "pdp11.h";
#include "memory.h";

//Function definitions
int Fetch_Decode(int RAM [], instruction & current_inst) {

  int addr = RAM;

  //single operand instruction
  if (((addr >> 0xc) & 0x0f) == 0x1) {
    
    
  }

  //conditional branch instruction
  else if (((addr >> 0xc) & 0x0f) == 0x0) {
    

  }

  //double operand special operation
  else if (((addr >> 0xd) & 0x07) == 0x7) {

  }

  //double operand instruction
  else {

  }
	
  //TODO Jordan implement decode and populate current_inst
  //Use OpCode Mnemonics
}

int Write_mem(int & result, int & dest_addr) {

  //TODO Jordan implement writes to memory
}
