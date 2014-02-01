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
int Fetch_Decode(int RAM [], int GPR [], instruction & current_inst, ofstream & file, bool I_or_D) {
  I_or_D = true;
  int CurrentInst;
  CurrentInst = Read_mem(RAM, GPR, file, I_or_D);

  if (CurrentInst == 0x5){
      //RESET CODE HERE...
  }
  
  // I dont believe mnemonics work for this op because there can be any 
  // combo of the four being set or cleared...
  /*
  else if (((CurrentInst & 0xffe0) >> 0x5) == 0x5) {
    
    if ((CurrentInst & 0x10) == 0x10){
    
      if ((CurrentInst & 0x1) == 0x1)
        Status_word.C = true;
    
      if ((CurrentInst & 0x2) == 0x2)
        Status_word.V = true;
    
      if ((CurrentInst & 0x4) == 0x4)
        Status_word.Z = true;
    
      if ((CurrentInst & 0x8) == 0x8)
        Status_word.N = true;
    }
      
    else if ((CurrentInst & 0x10) == 0x0){
      
      if ((CurrentInst & 0x1) == 0x1)
        Status_word.C = false;
      
      if ((CurrentInst & 0x2) == 0x2)
      Status_word.V = false;
      
      if ((CurrentInst & 0x4) == 0x4)
        Status_word.Z = false;
        
      if ((CurrentInst & 0x8) == 0x8)
        tatus_word.N = false;
    }
  }*/

	//Decode the current instruction
	/* -- jump instruction -- */
	else if (((CurrentInst & 0x40) >> 0x6) == 0x1) {
		current_inst.instSel = JUMP;
		current_inst.offset = ((CurrentInst & 0x3f) - 1);
	}
	
  /* -- single operand instruction -- */
  else if (((CurrentInst & 0x7800) >> 0xb)  == 0x1) {
    I_or_D = false;
    current_inst.instSel = SINGLE_OP;

    current_inst.byteSel = ((CurrentInst & 0x8000) >> 0xe);
    current_inst.opcode = ((CurrentInst & 0x7c0) >> 0x6);
    current_inst.modeDest = ((CurrentInst & 0x38) >> 0x3);    

    if ((current_inst.modeDest == 0x6) || (current_inst.modeDest == 0x7))
      current_inst.regster = Read_mem(RAM, GPR, file, I_or_D);
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
    I_or_D = false;
    current_inst.instSel = DOUBLE_OP_SP;

    current_inst.opcode = ((CurrentInst & 0xe00) >> 0x9);
    current_inst.modeDest = ((CurrentInst & 0x38) >> 0x3);

    if ((current_inst.modeSrc == 0x6) || (current_inst.modeSrc == 0x7))
      current_inst.source = Read_mem(RAM, GPR, file, I_or_D);
    else
      current_inst.source = ((CurrentInst & 0x1c0) >> 0x6);
      
    if ((current_inst.modeDest == 0x6) || (current_inst.modeDest == 0x7))
      current_inst.destination = Read_mem(RAM, GPR, file, I_or_D); 
    else
      current_inst.destination = (CurrentInst & 0x3);   
  }

  /* -- Double operand instruction -- */
  else {
    I_or_D = false;
    current_inst.instSel = DOUBLE_OP;

    current_inst.opcode = ((CurrentInst & 0x7000) >> 0xc);
    current_inst.byteSel = ((CurrentInst & 0x8000) >> 0xe);
    current_inst.modeSrc = ((CurrentInst & 0xe00) >> 0x9);
    current_inst.modeDest = ((CurrentInst & 0x38) >> 0x3);

    if ((current_inst.modeSrc == 0x6) || (current_inst.modeSrc == 0x7))
      current_inst.source = Read_mem(RAM, GPR, file, I_or_D);
    else
      current_inst.source = ((CurrentInst & 0x1c0) >> 0x6);
      
    if ((current_inst.modeDest == 0x6) || (current_inst.modeDest == 0x7))
      current_inst.destination = Read_mem(RAM, GPR, file, I_or_D); 
    else
      current_inst.destination = (CurrentInst & 0x3); 
  }
}

//This function will be used to fetch instructions or data from memory
int Read_mem(int RAM [], int GPR [], ofstream & file, bool I_or_D) {
  
  int Address;

  Address = RAM[(GPR[7])];
  GPR[7] = (GPR[7] + 1);
  Address = ((Address << 0x8) | RAM[GPR[7]]);
  GPR[7] = (GPR[7] + 1);
  
  if (I_or_D)
    file << "2" << "\t" << Address << '\n';
  else
    file << "0" << "\t" << Address << '\n';
  
  return Address;
}


int Write_mem(int RAM [], int & result, int & dest_addr, ofstream & file) {
  
  RAM[dest_addr] = (result & 0xff);
  RAM[(dest_addr + 1)] = (result & 0xff00);
  
  file << "1" << "\t" << dest_addr << '\n';
  
  return 1;
}


