//******************************************************************************
// memory.cpp
//
//  Created on: Jan 20, 2014
//      Author: Rob Gaskell
// 	   Project: pdp11
//******************************************************************************

#include "memory.h"

//Function definitions
void Fetch_Decode(instruction & current_inst, bool I_or_D) {
  current_inst.Op_flag = true;
  I_or_D = true;
  int CurrentInst;
  CurrentInst = access_mem(GPR[7], FETCH);
  GPR[7] = GPR[7] + 2;

  //HALT op
  if (CurrentInst == 0x0) {
    current_inst.opcode = 0x0;
    current_inst.Op_flag = false;
  }

  /* -- Double operand instruction -- */
  else if (CurrentInst & 0x7000) {
    I_or_D = false;
    current_inst.instSel = DOUBLE_OP;

    current_inst.opcode = ((CurrentInst & 0x7000) >> 0xc);
    current_inst.byteSel = ((CurrentInst & 0x8000) >> 0xf);
    current_inst.modeSrc = ((CurrentInst & 0xe00) >> 0x9);
    current_inst.modeDest = ((CurrentInst & 0x38) >> 0x3);
    current_inst.sourceReg = ((CurrentInst & 0x1c0) >> 0x6);
    current_inst.destReg = (CurrentInst & 0x7);

    //PC operation -- src
    if ((current_inst.sourceReg == PC) || (current_inst.modeSrc > 0x5)) {
    	int16_t temp16bit = access_mem(GPR[7], FETCH);
  		GPR[7] = GPR[7] + 2;
      current_inst.source = temp16bit;
      current_inst.srcPC = (GPR[PC]);
    }

    //PC operation -- dst
    if ((current_inst.destReg == PC) || (current_inst.modeDest > 0x5)) {
      int16_t temp16bit = access_mem(GPR[7], FETCH);
  		GPR[7] = GPR[7] + 2;
      current_inst.destination = temp16bit;
      current_inst.destPC = (GPR[PC]);
    }
  }

  /* -- single operand instruction -- */
  else if ((CurrentInst & 0x7800)  == 0x800) {
    if (((CurrentInst & 0xfc0) >> 0x6) > 0x27) {
      I_or_D = false;
      current_inst.instSel = SINGLE_OP;

      current_inst.byteSel = ((CurrentInst & 0x8000) >> 0xf);
      current_inst.opcode = ((CurrentInst & 0x7c0) >> 0x6);
      current_inst.modeDest = ((CurrentInst & 0x38) >> 0x3);
      current_inst.destReg = (CurrentInst & 0x7);

      //PC operation
      if ((current_inst.destReg == PC) || (current_inst.modeDest > 0x5)) {
        int16_t temp16bit = access_mem(GPR[7], FETCH);
  			GPR[7] = GPR[7] + 2;
      	current_inst.destination = temp16bit;
        current_inst.destPC = (GPR[PC]);
      }
    }

      //JSR
    else if (((CurrentInst & 0xfc0) >> 0x6) < 0x28) {
      current_inst.instSel = JUMP;
      current_inst.modeDest = regADD;
      current_inst.byteSel = ((CurrentInst & 0x8000) >> 0xf);
      current_inst.opcode = JSR;
      
      current_inst.destReg = PC;
      
      current_inst.modeSrc = regS;
      current_inst.sourceReg = ((CurrentInst & 0x01c0) >> 0x6);
      
      current_inst.destReg = SP;
      current_inst.destPC = (GPR[PC]);
      int16_t temp16bit = access_mem(GPR[7], FETCH);
  		GPR[7] = GPR[7] + 2;
      current_inst.destination = temp16bit;
    }
  }

  /* -- Conditional branch instruction -- */
  else if ((CurrentInst & 0x7800) == 0x0) {
    if (((CurrentInst & 0x700) > 0x0) || ((CurrentInst & 0xf800) == 0x8000)) {
    	current_inst.byteSel = ((CurrentInst & 0x8000) >> 0xf);
      current_inst.instSel = CONDITIONAL_OP;
      
      current_inst.opcode = ((CurrentInst & 0x700) >> 0x8);
      if (current_inst.opcode == 0x0)
      	current_inst.opcode = BPL;
      	
			int8_t temp8bit = ((CurrentInst & 0xff));
			current_inst.offset = temp8bit;
    }
    
    //Set status word opertation
    else if ((CurrentInst & 0xffe0) == 0xa0) {
      current_inst.Op_flag = false;
      current_inst.opcode = 0x1;
      if ((CurrentInst & 0x10) == 0x10) {

        if ((CurrentInst & 0x1) == 0x1)
          Status_word.C = true;

        if ((CurrentInst & 0x2) == 0x2)
          Status_word.V = true;

        if ((CurrentInst & 0x4) == 0x4)
          Status_word.Z = true;

        if ((CurrentInst & 0x8) == 0x8)
          Status_word.N = true;
      }

      else if ((CurrentInst & 0x10) == 0x0) {

        if ((CurrentInst & 0x1) == 0x1)
          Status_word.C = false;

        if ((CurrentInst & 0x2) == 0x2)
          Status_word.V = false;

        if ((CurrentInst & 0x4) == 0x4)
          Status_word.Z = false;

        if ((CurrentInst & 0x8) == 0x8)
          Status_word.N = false;
      }
    }

    //RTS
    else if ((CurrentInst & 0xfff8) == 0x0080) {
    	current_inst.instSel = JUMP;
     	current_inst.opcode = RTS;
 
     	//Set Link Register Fields
     	current_inst.modeDest = regS;
     	current_inst.destReg = (CurrentInst & 0x0007);
 
     	//Set Stack Pointer Fields
     	current_inst.modeSrc = regAI;
     	current_inst.sourceReg = SP;
    }

	  //Decode the current instruction
	  /* -- jump instruction -- */
	  else if (((CurrentInst & 0x40)) == 0x40) {
		  current_inst.instSel = JUMP;
		  int16_t temp16bit = 0;
		  current_inst.modeDest = ((CurrentInst & 0x38) >> 0x3);
      current_inst.destReg = (CurrentInst & 0x7);
		  
		  //current_inst.offset = (CurrentInst & 0x3f);
		  
		  if ((current_inst.destReg == PC) || (current_inst.modeDest > 0x5)) {
      	temp16bit = access_mem(GPR[7], FETCH);
  			GPR[7] = GPR[7] + 2;
      	current_inst.destination = temp16bit;
      	current_inst.destPC = (GPR[PC]);
    	}
		  
	  }

    //RESET op
    else if (CurrentInst == 0x5){
        //RESET CODE HERE...
    }

  }
}

//This function will be used to fetch instructions or data from memory
int access_mem(int index, int flag, int out=0) {

  int value = 0;

  if (flag == READ){
    file << "0" << "\t" << setfill('0') << setw(6) << oct << index << '\n';
    value = RAM[index];
    value = (value | (RAM[index + 1] << 0x8));
  }

  else if (flag == WRITE){
    RAM[index] = (out & 0xFF);
    RAM[index + 1] = ((out & 0xFF00) >> 0x8);
    file << "1" << "\t" << setfill('0') << oct << setw(6) <<  index << '\n';
  }

  else if (flag == FETCH){
    file << "2" << "\t" << setfill('0') << oct << setw(6) <<  index << '\n';
    value = RAM[index];
    value = (value | (RAM[index + 1] << 0x8));

    if (display){
      // dump register values
      for (int i=0; i<REGISTERS; ++i){
        cout << "R" << i << "," 
             << setfill('0') << oct << setw(6) << GPR[i] << endl;
      }
      // dump PSW 
      cout << Status_word.priority << Status_word.T
           << Status_word.Z << Status_word.N << Status_word.C
           << Status_word.V << endl;
      // dump fetched instruction
      cout << "\t" << setfill('0') << oct << setw(6) << value << endl;
    }
  }

  else{
    value = RAM[index];
    value = (value | (RAM[index + 1] << 0x8));
  }

  return value;
}

