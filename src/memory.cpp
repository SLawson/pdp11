//******************************************************************************
// memory.cpp
//
//  Created on: Jan 20, 2014
//      Author: Rob Gaskell
// 	   Project: pdp11
//******************************************************************************

#include "pdp11.h"
#include "memory.h"

//Function definitions
int Fetch_Decode(int RAM [], instruction & current_inst) {

//--not complete but it compiles--
  
  //single operand instruction
  if (((*RAM >> 0xc) & 0x0f) == 0x1) {
    singleOpInst currentInst;
    currentInst.reg = (*RAM & 0x7);
    currentInst.mode = ((*RAM & 0x38) >> 0x3);
    currentInst.opCode = ((*RAM & 0x7c0) >> 0x6);
    currentInst.byteSel = ((*RAM & 0x8000) >> 0x9);
  }

  //conditional branch instruction
  else if (((*RAM >> 0xc) & 0x0f) == 0x0) {
    condBrInst currentInst;
    currentInst.offset = (*RAM & 0xff);
    currentInst.opCode = ((*RAM & 0x700) >> 0xf);
  }

  //double operand special operation
  else if (((*RAM >> 0xd) & 0x07) == 0x7) {
    doubleOpSpInst currentInst;
    currentInst.src_dest = (*RAM & 0x7);
    currentInst.mode = ((*RAM & 0x38) >> 0x3);
    currentInst.reg = ((*RAM & 0x1c0) >> 0x6);
    currentInst.opCode = ((*RAM & 0xe00) >> 0x9);
  }

  //double operand instruction
  else {
    doubleOpInst currentInst;
    currentInst.destination = (*RAM & 0x7);
    currentInst.modeB = ((*RAM & 0x38) >> 0x3);
    currentInst.source = ((*RAM & 0x1c0) >> 0x6);
    currentInst.modeA = ((*RAM & 0xe00) >> 0x9);
    currentInst.opCode = ((*RAM & 0x7000) >> 0xc);
    currentInst.opCode = ((*RAM & 0x8000) >> 0xf);
  }
	
  //TODO Jordan implement decode and populate current_inst
  //Use OpCode Mnemonics
}

int Write_mem(int & result, int & dest_addr) {

  //TODO Jordan implement writes to memory
}


