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
int Fetch_Decode(int RAM [], int GPR [], instruction & current_inst, ofstream & file) {
  
  int CurrentInst;
  CurrentInst = fetch(RAM, GPR, file);

	//Decode the current instruction
	/* -- jump instruction -- */
	if (((CurrentInst & 0x40) >> 0x6) == 0x1) {
		current_inst.instSel = JUMP;
		current_inst.offset = ((CurrentInst & 0x3f) - 1);
	}
	
  /* -- single operand instruction -- */
  else if (((CurrentInst & 0x7800) >> 0xb)  == 0x1) {
    current_inst.instSel = SINGLE_OP;

    current_inst.byteSel = ((CurrentInst & 0x8000) >> 0xe);
    current_inst.opcode = ((CurrentInst & 0x7c0) >> 0x6);
    current_inst.modeDest = ((CurrentInst & 0x38) >> 0x3);    

    current_inst.regster = Read_mem(RAM, GPR, file);
  }

  /* -- Conditional branch instruction -- */
  else if (((CurrentInst & 0x7800) >> 0xb) == 0x0) {
    current_inst.instSel = CONDITIONAL_OP;
    
    current_inst.opcode = ((CurrentInst & 0x700) >> 0x8);
    
    if ((CurrentInst & 0x80) == 0x80)
    	current_inst.offset = (0 - (CurrentInst & 0xff) - 1);
    else
    	current_inst.offset = ((CurrentInst & 0xff) - 1);
    
  }

  /* -- Double operand special operation -- */
  else if (((CurrentInst & 0x7000) >> 0xc) == 0x7) {
    current_inst.instSel = DOUBLE_OP_SP;

    current_inst.opcode = ((CurrentInst & 0xe00) >> 0x9);
    current_inst.modeDest = ((CurrentInst & 0x38) >> 0x3);

    current_inst.source = Read_mem(RAM, GPR, file);
    current_inst.destination = Read_mem(RAM, GPR, file);    
  }

  /* -- Double operand instruction -- */
  else {
    current_inst.instSel = DOUBLE_OP;

    current_inst.opcode = ((CurrentInst & 0x7000) >> 0xc);
    current_inst.byteSel = ((CurrentInst & 0x8000) >> 0xe);
    current_inst.modeSrc = ((CurrentInst & 0xe00) >> 0x9);
    current_inst.modeDest = ((CurrentInst & 0x38) >> 0x3);

    current_inst.source = Read_mem(RAM, GPR, file);
    current_inst.destination = Read_mem(RAM, GPR, file); 
  }
}

//This function will be used to fetch the next instruction or data
int fetch(int RAM [], int GPR [], ofstream & file) {

  int Instruction;
  
  Instruction = RAM[(GPR[7])];
  GPR[7] = (GPR[7] + 1);
  Instruction = ((Instruction << 0x8) | RAM[GPR[7]]);
  GPR[7] = (GPR[7] + 1);
  
  //file << "2" << "\t" << Instruction << '\n';
  
  return Instruction;
}

//This function will be used to fetch the data from memory
int Read_mem(int RAM [], int GPR [], ofstream & file) {

  int Address;
  
  Address = RAM[(GPR[7])];
  GPR[7] = (GPR[7] + 1);
  Address = ((Address << 0x8) | RAM[GPR[7]]);
  GPR[7] = (GPR[7] + 1);
  
  //file << "0" << "\t" << Address << '\n';
  
  return Address;
}


int Write_mem(int RAM [], int & result, int & dest_addr, ofstream & file) {
  
  RAM[dest_addr] = (result & 0xff);
  RAM[(dest_addr + 1)] = (result & 0xff00);
  
  //file << "1" << "\t" << dest_addr << '\n';
  
  return 1;
}


