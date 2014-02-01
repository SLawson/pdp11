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
#include "testing.h"

//Global variable for the trace file
ofstream file;

int main(int argc, char * argv[]) {

//Variables
instruction current_inst = {0,0,0,0,0,0,0,0,0,0,0,0,0};       //Fetched/decoded instruction
PSW Status_word = {0, false, false, false, false, false};     //Current PSW
int RAM[MEM_SIZE] = {0};                                      //Contents of main memory
int GPR[REGISTERS] = {0};                                     //General Purpose Registers
int init_status = 0;                                          //initialization routine return code
bool I_or_D = false;
string out_file("trace.txt");                                 //output trace file name
file.open("tracefile.txt");

  cout <<"PDP-11/20 Simulation\n\n";

  if(DEBUG_FLAG)
    cout <<"*************************DEBUG MODE*************************\n\n";

  //Initialize program
  init_status = initialize(RAM, argc, argv, &GPR[PC], out_file); //Read ASCII file, write code and static data to memory

  if (init_status) {
    return 0;
  }

  //Execute instructions
  do {
      Fetch_Decode(RAM, GPR, current_inst, file, I_or_D, Status_word);
      Operation(RAM,current_inst, GPR, Status_word);

      if(current_inst.write_flag) {      //Write to RAM if write_flag set
        Write_mem(RAM, current_inst.result, current_inst.dest_addr, file);
        current_inst.write_flag = false;
      }
  } while(current_inst.opcode || current_inst.byteSel);

  cout <<"\nPDP-11/20 Simulation Complete\n";
  reg_dump(GPR, &Status_word);
  file.close();
  cin.get();    //Pause before exiting
  return 0;
}

