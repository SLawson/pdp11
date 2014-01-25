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
  
  //single operand instruction
  if (((*RAM >> 0xc) & 0x0f) == 0x1) {
    current_inst.instSel = 1;
    current_inst.operand1 = (*RAM & 0x7);
    current_inst.modeDest = ((*RAM & 0x38) >> 0x3);
    current_inst.opcode = ((*RAM & 0x7c0) >> 0x6);
    current_inst.byteSel = ((*RAM & 0x8000) >> 0x9);
  }

  //conditional branch instruction
  else if (((*RAM >> 0xc) & 0x0f) == 0x0) {
    current_inst.instSel = 0;
    current_inst.offset = (*RAM & 0xff);
    current_inst.opcode = ((*RAM & 0x700) >> 0xf);
  }

  //double operand special operation
  else if (((*RAM >> 0xd) & 0x07) == 0x7) {
    current_inst.instSel = 3;
    current_inst.operand2 = (*RAM & 0x7);
    current_inst.modeDest = ((*RAM & 0x38) >> 0x3);
    current_inst.operand1 = ((*RAM & 0x1c0) >> 0x6);
    current_inst.opcode = ((*RAM & 0xe00) >> 0x9);
  }

  //double operand instruction
  else {
    current_inst.instSel = 2;
    current_inst.operand2 = (*RAM & 0x7);
    current_inst.modeDest = ((*RAM & 0x38) >> 0x3);
    current_inst.operand1 = ((*RAM & 0x1c0) >> 0x6);
    current_inst.modeSrc = ((*RAM & 0xe00) >> 0x9);
    current_inst.opcode = ((*RAM & 0x7000) >> 0xc);
    current_inst.byteSel = ((*RAM & 0x8000) >> 0xf);
  }
	
  //TODO Jordan implement decode and populate current_inst
  //Use OpCode Mnemonics
}

int Write_mem(int & result, int & dest_addr) {

  //TODO Jordan implement writes to memory
}


