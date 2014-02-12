//******************************************************************************
// memory.cpp
//
//  Created on: Jan 20, 2014
//      Author: Rob Gaskell
// 	   Project: pdp11
//******************************************************************************

#include "memory.h"

//Function definitions
void Fetch_Decode(int RAM [], int GPR [], instruction & current_inst, ofstream & file, bool I_or_D, PSW & Status_word) {
  current_inst.Op_flag = true;
  I_or_D = true;
  int CurrentInst;
  CurrentInst = Read_mem(RAM, GPR, file, I_or_D, &Status_word);

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
      current_inst.source = Read_mem(RAM, GPR, file, I_or_D, &Status_word);
      current_inst.srcPC = (GPR[PC]);
    }

    //PC operation -- dst
    if ((current_inst.destReg == PC) || (current_inst.modeDest > 0x5)) {
      current_inst.destination = Read_mem(RAM, GPR, file, I_or_D, &Status_word);
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
        current_inst.destination = Read_mem(RAM, GPR, file, I_or_D, &Status_word);
        current_inst.srcPC = (GPR[PC]);
      }
    }

      //JSR
    else if (((CurrentInst & 0xfc0) >> 0x6) < 0x28) {
      current_inst.instSel = JUMP;
      current_inst.modeDest = regAD;
      current_inst.destReg = ((CurrentInst & 0x01c0) >> 0x6);
      current_inst.destination = (CurrentInst & 0x003f); //this is the value of the 6 bit dst field...
    }
  }

  /* -- Conditional branch instruction -- */
  else if ((CurrentInst & 0x7800) == 0x0) {
    if (((CurrentInst & 0x700) >> 0x8) > 0x0) {
    	int8_t temp8bit = 0;
      current_inst.instSel = CONDITIONAL_OP;
      current_inst.opcode = ((CurrentInst & 0x700) >> 0x8);
			temp8bit = ((CurrentInst & 0xff));
			current_inst.offset = temp8bit;

      /*if ((CurrentInst & 0x80) == 0x80) {
      	current_inst.offset = (0 - (CurrentInst & 0xff) - 1);

      else
      	current_inst.offset = ((CurrentInst & 0xff) - 1);*/
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
      current_inst.modeSrc = regAI;
      current_inst.sourceReg = (CurrentInst & 0x0007);
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
      	temp16bit = Read_mem(RAM, GPR, file, I_or_D, &Status_word);
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
int Read_mem(int RAM [], int GPR [], ofstream & file, bool I_or_D, PSW* Status_word) {

  int Address;

  if (I_or_D){
    if (display){
      status_dump(GPR, Status_word);
    }
  }

  Address = RAM[(GPR[PC])];
  GPR[PC] = (GPR[PC] + 1);
  Address = ((Address) | (RAM[GPR[PC]] << 0x8));
  GPR[PC] = (GPR[PC] + 1);

  if (I_or_D){
    file << "2" << "\t" << setfill('0') << oct << setw(6) <<  Address << '\n';

    if (display){
      cout << "\t" << setfill('0') << oct << setw(6) << Address  << endl;
    }
  }

  else
    file << "0" << "\t" << setfill('0') << setw(6) << oct << Address << '\n';

  return Address;
}


int Write_mem(int RAM [], int & result, int & dest_addr, ofstream & file) {

  RAM[dest_addr] = (result & 0xff);
  RAM[(dest_addr + 1)] = ((result & 0xff00) >> 0x8);

  file << "1" << "\t" << oct << setw(6) << dest_addr << '\n';


  return 1;
}


