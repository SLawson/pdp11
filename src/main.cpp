//******************************************************************************
// main.cpp
//
//  Created on: Jan 20, 2014
//      Author: Rob Gaskell
// 	   Project: pdp11
//******************************************************************************

#include "pdp11.h"
#include "memory.h"
#include "initialize.h"
#include "operations.h"

int main() {

//Variables
instruction current_inst = {0,0,0,0,0,0};       //Fetched/decoded instruction
PSW Status_word = {0, false, false, false, false, false};       //Current PSW
int RAM[MEM_SIZE];                              //Contents of main memory
int GPR[REGISTERS];                             //General Purpose Registers

  cout <<"PDP-11/20 Simulation\n\n";

  if(DEBUG_FLAG)
    cout <<"*************************DEBUG MODE*************************\n\n";

  //Initialize program
  initialize(RAM); //Read ASCII file, write code and static data to memory

  //Execute instructions
  while(current_inst.opcode) {
      Fetch_Decode(RAM, current_inst);
      Operation(current_inst, GPR, Status_word);

      if(current_inst.write_flag)       //Write to RAM if write_flag set
        Write_mem(current_inst.result, current_inst.dest_addr);
  }

  cout <<"\nPDP-11/20 Simulation Complete\n";
  cin.get();    //Pause before exiting
  return 0;
}
