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
int Fetch_Decode(int RAM [], int GPR [], instruction & current_inst) {
  
  //fetch the current instruction
  int CurrentInst = RAM[(GPR[7])];
  GPR[7] = (GPR[7] + 1);
  CurrentInst = ((CurrentInst << 0x8) | RAM[(GPR[7])]);

	//decode the current instruction
	if (((CurrentInst & 0x40) >> 0x6) == 0x1) {
		current_inst.instSel = 4;
		
		current_inst.offset = ((CurrentInst & 0x3f) - 1);
	}
	
  //single operand instruction
  else if (((CurrentInst & 0x7800) >> 0xb)  == 0x1) {
    current_inst.instSel = 1;

    current_inst.byteSel = ((CurrentInst & 0x8000) >> 0xe);
    current_inst.opcode = ((CurrentInst & 0x7c0) >> 0x6);
    current_inst.modeDest = ((CurrentInst & 0x38) >> 0x3);    
    
    GPR[7] = (GPR[7] + 1);
    current_inst.regster = RAM[GPR[7]];
    GPR[7] = (GPR[7] + 1);
    current_inst.regster = ((current_inst.regster << 0x8) | RAM[(GPR[7])]);
  }

  //conditional branch instruction
  else if (((CurrentInst & 0x7800) >> 0xb) == 0x0) {
    current_inst.instSel = 0;
    current_inst.opcode = ((CurrentInst & 0x700) >> 0x8);
    
    if ((CurrentInst & 0x80) == 0x80)
    	current_inst.offset = (0 - (CurrentInst & 0xff) - 1);
    else
    	current_inst.offset = ((CurrentInst & 0xff) - 1);
    
  }

  //double operand special operation
  else if (((CurrentInst & 0x7000) >> 0xc) == 0x7) {
    current_inst.instSel = 3;

    current_inst.opcode = ((CurrentInst & 0xe00) >> 0x9);
    current_inst.modeDest = ((CurrentInst & 0x38) >> 0x3);

    GPR[7] = (GPR[7] + 1);
    current_inst.source = RAM[GPR[7]];
    GPR[7] = (GPR[7] + 1);
    current_inst.source = ((current_inst.source << 0x8) | RAM[GPR[7]]);
    GPR[7] = (GPR[7] + 1);
    current_inst.destination = RAM[GPR[7]];
    GPR[7] = (GPR[7] + 1);
    current_inst.destination = ((current_inst.destination << 0x8) | RAM[GPR[7]]);    
  }

  //double operand instruction
  else {
    current_inst.instSel = 2;

    current_inst.opcode = ((CurrentInst & 0x7000) >> 0xc);
    current_inst.byteSel = ((CurrentInst & 0x8000) >> 0xe);
    current_inst.modeSrc = ((CurrentInst & 0xe00) >> 0x9);
    current_inst.modeDest = ((CurrentInst & 0x38) >> 0x3);
    
    GPR[7] = (GPR[7] + 1);
    current_inst.source = RAM[GPR[7]];
    GPR[7] = (GPR[7] + 1);
    current_inst.source = ((current_inst.source << 0x8) | RAM[GPR[7]]);
    GPR[7] = (GPR[7] + 1);
    current_inst.destination = RAM[GPR[7]];
    GPR[7] = (GPR[7] + 1);
    current_inst.destination = ((current_inst.destination << 0x8) | RAM[GPR[7]]); 
  }

  GPR[7] = (GPR[7] + 1);	
  
  //TODO Jordan implement decode and populate current_inst
  //Use OpCode Mnemonics
}

int Write_mem(int RAM [], int & result, int & dest_addr) {

  //TODO Jordan implement writes to memory
  
  RAM[dest_addr] = (result & 0xff);
  RAM[dest_addr] = (RAM[dest_addr] + 1);
  RAM[dest_addr] = (result & 0xff00);
}


