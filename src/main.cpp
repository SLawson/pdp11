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


//Global Variables
static PSW Status_word = {0, false, false, false, false, false};	//Current PSW
static int RAM[MEM_SIZE] = {0};	//Contents of main memory
static ofstream file;			// trace file stream


int main(int argc, char * argv[]) {

//Local Variables
instruction current_inst = {0};	//Fetched/decoded instruction
bool I_or_D = false;			//Instruction or Data Flag

//R6 (SP) is initialized to the top of accessible memory space, below IO space
int GPR[REGISTERS] = {0,0,0,0,0,0,61439,0};		//General Purpose Registers

int instruction_count = 0;

string out_file("tracefile.txt");	//output trace file name

	cout <<"PDP-11/20 Simulation\n\n";

	//Indicates Debug build
	if(DEBUG_FLAG) {
		cout <<"*************************DEBUG MODE*************************\n\n";
	}

	//Initialize program, exit if initialization fails
	if(initialize(argc, argv, &GPR[PC], out_file)) {	//Read ASCII file, write code and static data to memory
		return 0;
	{

	//Open tracefile for output
	file.open(out_file.c_str());

	//Fetch/Execute instructions
	do {
		Fetch_Decode(GPR, current_inst, I_or_D, Status_word);

		//If instruction requires further ALU operation
		if (current_inst.Op_flag)
			Operation(current_inst, GPR, Status_word);

		//Write result to RAM if write_flag set
		if(current_inst.write_flag) {
			Write_mem(current_inst.result, current_inst.dest_addr);
			current_inst.write_flag = false;
		}
		instruction_count = instruction_count + 1;	//Increment instructions executed count

	} while(current_inst.opcode);	//Continue until HALT instruction encountered

	//Display summary of simulation
	cout <<"\nPDP-11/20 Simulation Complete\n";
	cout <<"\nInstructions Executed: " <<instruction_count <<'\n';

	//Write processor state to file if filename specified
	reg_dump(&Status_word, 1, 0);

	file.close();	//Close trace file

	cin.get();		//Pause before exiting
	return 0;
}

