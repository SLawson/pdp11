//******************************************************************************
// operations.cpp
//
//  Created on: Jan 20, 2014
//      Author: Rob Gaskell		PSU ID# 914464617
// 	   Project: pdp11
//******************************************************************************

#include "pdp11.h"
#include "operations.h"
#include "memory.h"

void Operation(int RAM[],instruction & current_inst, int GPR [], PSW & Status_word) {

  //TODO Rob/Brett implement instruction ops, populate current_inst.result

    //Internal scratch pad registers
    int opsource;//this holds the value of the data (RAM or register)
    int opdestination;//this holds the value of the data (RAM or register)
    bool writeflag = true;//sets writeflag to alway write

    opdestination = AddressmodesDecode(RAM, current_inst.modeDest, current_inst.destination, GPR,current_inst.destReg,current_inst.destPC);

/*********************************************************************************
Double Operation

Take a source and destination memory location

*********************************************************************************/
  if(current_inst.instSel == DOUBLE_OP  && current_inst.byteSel == 0)
  {

    opsource = AddressmodesDecode(RAM, current_inst.modeSrc, current_inst.source, GPR, current_inst.sourceReg,current_inst.srcPC);


    //once the correct destination and source the operation can occur
    switch(current_inst.opcode){

      case MOV: //dest = src
      {

        opdestination = opsource;
        StatusFlags(Status_word,opdestination,0);
        Status_word.V = false;//sets the overflow flag to false
        break;

      }
      case CMP://compute src - dest, set flags only
      {
        int tempresult;
		    tempresult = opsource - opdestination;
		    StatusFlags(Status_word,tempresult,0);
		    writeflag = false;//does not modify memory/registers

        break;
      }
      case BIT://compute dest & src set flags only
      {
        int tempresult=opdestination & opsource;
        StatusFlags(Status_word,tempresult,0);
        Status_word.V = false;//sets the overflow flag to false
        writeflag = false;//does not modify memory/registers
        break;
      }
      case BIC://dest &= ~src
      {
        opdestination= ~opsource & opdestination;
        StatusFlags(Status_word,opdestination,0);
        Status_word.V = false;//sets the overflow flag to false
        break;

      }
      case BIS://Logical OR dest|=src
      {
        opdestination= opsource | opdestination;
        StatusFlags(Status_word,opdestination,0);
        Status_word.V = false;//sets the overflow flag to false
        break;

      }
      case ADD://dest +=src
      {
        opdestination += opsource;
        StatusFlags(Status_word,opdestination,1);
        break;
      }
      default:
      {
            cout << "error in double op\n";
            break;
      }
    }
  }
  else if(current_inst.instSel == DOUBLE_OP  && current_inst.byteSel == 1)
  {
	opsource = AddressmodesDecode(RAM, current_inst.modeSrc, current_inst.source, GPR, current_inst.sourceReg,current_inst.srcPC);
	switch(current_inst.opcode){

		case SUB://dest -=src
		{
		opdestination -= opsource;
		StatusFlags(Status_word,opdestination,1);
		break;
		}
		default:
        {
            cout << "error in double op\n";
            break;
        }
	}
  }

  /*********************************************************************
  Single operand instructions

  Only takes destination and modifies Destination

  *********************************************************************/

  else if(current_inst.instSel == SINGLE_OP && current_inst.byteSel == 0)
  {
    switch(current_inst.opcode)
    {
      case CLR://set the destination to zero
      {
        opdestination = 0;
        Status_word.Z = true;//sets the zero flag to true
        Status_word.N = false;//sets the negative flag to false
        Status_word.C = false;//sets the carry flag to false
        Status_word.V = false;//sets the overflow flag to false
        break;
      }
      case COM://sets the destinations complement
      {
        int16_t dest16 = ~opdestination;
        opdestination = dest16;
        StatusFlags(Status_word,opdestination,0);//just sets the zero and negative
        Status_word.C = false;//sets the carry flag to false
        Status_word.V = false;//sets the overflow flag to false
        break;
      }
      case INC://increments the destination value by one
      {
        opdestination = opdestination + 1;
        StatusFlags(Status_word,opdestination,0);//just sets the zero and negative
        break;
      }
      case DEC://decrements the destination value by one
      {
        opdestination = opdestination - 1;
        StatusFlags(Status_word,opdestination,0);//just sets the zero and negative
        break;
      }
      case NEG://changes the value to negative
      {
        opdestination = ~opdestination + 1;
        StatusFlags(Status_word,opdestination,1);//just sets the zero and negative
        break;
      }
      case ADC:
      {
        opdestination = opdestination + Status_word.C;
        StatusFlags(Status_word,opdestination,0);//just sets the zero and negative
        if(opdestination == -1)
        {
          Status_word.C = true;//sets the carry flag to true
        }
        break;
      }
      case SBC:
      {
        opdestination = opdestination - Status_word.C;
        StatusFlags(Status_word,opdestination, 0);//just sets the zero and negative
        if(opdestination == 0 && Status_word.C == true)
        {
            Status_word.C = false;//sets the carry flag to true
        }
        break;
      }
      case TST:
      {
        StatusFlags(Status_word,opdestination,0);//just sets the zero and negative
        Status_word.C = false;//sets the carry flag to false
        Status_word.V = false;//sets the overflow flag to false
        break;
      }
      case ROR:
      {
        int16_t dest16 = opdestination;
        int holder = (dest16 & 0x1);

        dest16 = (dest16 >> 0x1);
        if (Status_word.C)
          dest16 = (dest16 | (Status_word.C << 0xf));
        else if (!Status_word.C)
          dest16 = (dest16 & 0x7fff);
        Status_word.C = holder;

        //set remaining flags
        if (dest16 == 0)
          Status_word.Z = true;
        else
          Status_word.Z = false;
        if ((dest16 & 0x8000) == 0x8000)
          Status_word.N = true;
        else
          Status_word.N = false;
        Status_word.V = (Status_word.C ^ Status_word.N);
        opdestination = dest16;
        //current_inst.write_flag = true;
        break;
      }
      case ROL:
      {
        int16_t dest16 = opdestination;
        int holder = (dest16 & 0x8000);

        dest16 = (dest16 << 0x1);
        if (Status_word.C)
          dest16 = (dest16 | (Status_word.C));
        else if (!Status_word.C)
          dest16 = (dest16 & 0xfffe);
        Status_word.C = holder;

        //set remaining flags
        if (dest16 == 0)
          Status_word.Z = true;
        else
          Status_word.Z = false;
        if ((dest16 & 0x8000) == 0x8000)
          Status_word.N = true;
        else
          Status_word.N = false;
        Status_word.V = (Status_word.C ^ Status_word.N);
        opdestination = dest16;
        //current_inst.write_flag = true;
        break;
      }
      case ASR:
      {
        int16_t dest16 = opdestination;

        Status_word.C = (dest16 & 0x1);
        if (dest16 & 0x8000)
          dest16 = ((dest16 >> 0x1) | (0x1 << 0xf));
        else if (!(dest16 & 0x8000))
          dest16 = ((dest16 >> 0x1) & (0x7fff));

        //set remaining flags
        if (dest16 == 0)
          Status_word.Z = true;
        else
          Status_word.Z = false;

        if ((dest16 & 0x8000) == 0x8000)
          Status_word.N = true;
        else
          Status_word.N = false;

        Status_word.V = (Status_word.C ^ Status_word.N);
        opdestination = dest16;
        //current_inst.write_flag = true;
        break;
      }
      case ASL:
      {
        int16_t dest16 = opdestination;

        Status_word.C = (dest16 & 0x8000);
        if (dest16 & 0x8000)
          dest16 = ((dest16 << 0x1) | (0x1 << 0xf));
        else if (!(dest16 & 0x8000))
          dest16 = ((dest16 << 0x1) & (0xfffe));

        //set remaining flags
        if (dest16 == 0)
          Status_word.Z = true;
        else
          Status_word.Z = false;

        if ((dest16 & 0x8000) == 0x8000)
          Status_word.N = true;
        else
          Status_word.N = false;

        Status_word.V = (Status_word.C ^ Status_word.N);
        opdestination = dest16;
        //current_inst.write_flag = true;
        break;
      }
      case SWAB:
      {
        int16_t dest16 = opdestination;

        dest16 = ((dest16 >> 0x8) | ((dest16 & 0xff) << 0x8));

        //set flags
        if ((dest16 & 0xff) == 0)
          Status_word.Z = true;
        else
          Status_word.Z = false;

        if (((dest16 & 0x80) == 0x80)<< 0x8)
          Status_word.N = true;
        else
          Status_word.N = false;

        Status_word.C = false;
        Status_word.V = false;
        opdestination = dest16;
        //current_inst.write_flag = true;
        break;
      }
      default:
      {
          cout << "error in single op\n";
          break;
      }
    }
  }

  //Subroutine Jump Instructions
  else if(current_inst.instSel == JUMP) {

	  if(current_inst.opcode == JSR) {		//Jump to Subroutine

		  //Push specified Link Register's contents onto stack
		  //modeSrc = 00, sourceReg = Link Register
		  opdestination = AddressmodesDecode(RAM, current_inst.modeSrc, current_inst.source, GPR, current_inst.sourceReg,current_inst.srcPC);

		  //Copy PC's contents to specified Link Register (pre-incremented PC)
		  GPR[current_inst.destReg] = GPR[PC];

		  //Copy Jump Destination to PC
		  GPR[PC] = current_inst.destination;
	  }
	  else if(current_inst.opcode == RTS) {	//ReTurn from Subroutine

		  //Copy specified Link Register's contents to PC
		  GPR[PC] = GPR[current_inst.destReg];

		  //Pop top of stack to specified Link Register
		  opdestination = AddressmodesDecode(RAM, current_inst.modeSrc, current_inst.source, GPR, current_inst.sourceReg,current_inst.srcPC);
	  }
	  else {
		  //cout <<"Invalid Subroutine Instruction"; --move this!!!
			//Push specified Link Register's contents onto stack
		  //modeSrc = 00, sourceReg = Link Register
		  opdestination = AddressmodesDecode(RAM, current_inst.modeDest, current_inst.destination, GPR, current_inst.destReg,current_inst.srcPC);

		  //Copy PC's contents to specified Link Register (pre-incremented PC)
		  //GPR[current_inst.destReg] = GPR[PC];

		  //Copy Jump Destination to PC
		  GPR[PC] = opdestination;
			}
	  if(current_inst.byteSel == 1) {

		  cout <<"Erroneous B-bit";
	  }

  }

  //Conditional Branch Instructions
  //byteSel = 0: BR_JMP, BEQ, BNE, and Signed Conditional Branches
  if(current_inst.instSel == CONDITIONAL_OP && current_inst.byteSel == 0) {
	switch(current_inst.opcode) {
		case BR_JMP: {			//Unconditional Branch/Jump

			TakeBranch(GPR[PC], current_inst.offset);
			break;
		}
		case BNE: {				//Branch on Not Equal(Zero)

			if(Status_word.Z == false) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		case BEQ: {				//Branch on Equal(Zero)

			if(Status_word.Z == true) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		case BGE: {				//Branch on Greater than or Equal(Zero)

			if(Status_word.N == Status_word.V) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		case BLT: {				//Branch on Less Than(Zero)

			if(Status_word.N != Status_word.V) {

				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		case BGT: {				//Branch on Greater Than(Zero)

			if((Status_word.N == Status_word.V) || Status_word.Z == true) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		case BLE: {				//Branch on on Less than or Equal(Zero)

			if((Status_word.N != Status_word.V) || Status_word.Z == true) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		default: {				//Invalid OpCode/byteSel values for CONDITIONAL_OP

		  cout << "Invalid Conditional Branch Instruction\n";
		  break;
		}
	}
	writeflag = false;
  }
  //byteSel = 1: BMI, BPL, BCS, BCC, BVS, BVC, and Unsigned Conditional Branches
  else if(current_inst.instSel == CONDITIONAL_OP && current_inst.byteSel == 1) {
	switch(current_inst.opcode) {
		case BPL: {				//Branch on PLus

			if(Status_word.N == false) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		case BMI: {				//Branch on MInus

			if(Status_word.N == true) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		case BHI: {				//Branch on HIgher

			if(Status_word.C == false && Status_word.Z == false) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		case BLOS: {			//Branch on LOwer or Same

			if(Status_word.C == true || Status_word.Z == true) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		case BVC: {				//Branch on oVerflow Clear

			if(Status_word.V == false) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		case BVS: {				//Branch on oVerflow Set

			if(Status_word.V == true) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		case BCC_BHIS: {		//Branch on Carry Clear/Branch on HIgher or Same

			if(Status_word.C == false) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		case BCS_BLO: {			//Branch on Carry Set/Branch on LOwer

			if(Status_word.C == true) {
				TakeBranch(GPR[PC], current_inst.offset);
			}
			break;
		}
		default: {				//Invalid OpCode/byteSel values for CONDITIONAL_OP

		  cout << "Invalid Conditional Branch Instruction\n";
		  break;
		}
	}
	writeflag = false;
  }


/*******************************************************************
Write back to RAM or appropriate register
Only modifies the Destination works for both Double operand
and single operand

writeflag is used to check for a write for both memory or register

*******************************************************************/

  //return function here for the different modes
  if(writeflag)//checks if op requires write to memory or register
  {
      int RAM_contents;

      switch (current_inst.modeDest) {

        case regID:{//ID 7 Index deferred

            current_inst.write_flag = true;
            current_inst.result = opdestination;
            RAM_contents = RAM[(0xffff & (current_inst.destination + GPR[current_inst.destReg]))];//Read_mem(RAM, GPR, file, I_or_D) trying to add
            RAM_contents = ((RAM_contents) | (RAM[(0xffff &(current_inst.destination + GPR[current_inst.destReg]))+1] << 0x8));//joins the data with the lower 8 bits
            current_inst.dest_addr =  RAM[RAM_contents];//destination address;
            break;
        }
        case regI:{//ID6 Index
            current_inst.write_flag = true;
            current_inst.result = opdestination;
            current_inst.dest_addr = (0xffff &(current_inst.destination + GPR[current_inst.destReg])); //destination address
            break;
        }
        case regADD:{//ID5 Autodecrement deferred
            current_inst.write_flag = true;
            current_inst.result = opdestination;//DATA
            current_inst.dest_addr = RAM[GPR[current_inst.destReg]];
            break;
        }
        case regAD:{//ID4 Autodecrement
            current_inst.write_flag = true;
            current_inst.result = opdestination;
            current_inst.dest_addr = GPR[current_inst.destReg]; //operand
            break;
            }
        case regAID:{//ID3 Autoincrement deferred
                if(current_inst.destReg == SP)
                    GPR[current_inst.destReg] = opdestination;
                else
                {
                    current_inst.write_flag = true;
                    current_inst.result = opdestination;
                    current_inst.dest_addr = RAM[GPR[current_inst.destReg]-2];
                    break;
                }

            }
        case regAI:{//ID 2 Autoincrement
                    current_inst.write_flag = true;
                    current_inst.result = opdestination;
                    current_inst.dest_addr = GPR[current_inst.destReg]-2;

            break;
        }
        case regD://ID 1 Register Deferred
        {
            RAM[GPR[current_inst.destReg]] = opdestination;//Stores the address of the operand
            break;
        }
        case regS://ID 0 Register
        {
            GPR[current_inst.destReg] = opdestination;//stores the operand into the register
            break;
        }
        //need to add the rest of the modes here for double op
        default:
        {
            cout << "not a valid mode\n";
            break;
        }

      }
  }

}

/***************************************************************
Address modes handler



***************************************************************/

int AddressmodesDecode(int RAM[],int mode,int &address_op, int GPR[], int curr_Register, int prog_cntr) {

int operand_data=0;
int16_t address_location=0;


	//Takes the destination address and fetches the data from the RAM for the destination value
	switch (mode) {
		case regID: {	//Index Deferred Modes modeid:7

			//PC-relative Deferred Mode 77
			if(curr_Register == PC) {
			  //adds the PC to the address we are currently on to give us memory location
        address_op = (0xff & (address_op + prog_cntr));
        //takes the upper 8 bits of data from RAM]
				operand_data = RAM[address_op];
				//joins the data with the lower 8 bits giving the address
				operand_data = (operand_data | (RAM[address_op+1] << 0x8));
				//takes the upper 8 bits of data from RAM]
				operand_data = RAM[operand_data];
				//joins the data with the lower 8 bits giving the address
				address_location = ((operand_data) | (RAM[address_location+1] << 0x8));
				//returns the address of the address
				return address_location;
			}

			else {
				address_location = RAM[(0xffff & (address_op + GPR[curr_Register]))];//Read_mem(RAM, GPR, file, I_or_D) trying to add
                address_location = ((address_location) | (RAM[(0xffff &(address_op + GPR[curr_Register]))+1] << 0x8));//joins the data with the lower 8 bits
				operand_data = RAM[address_location];//Read_mem(RAM, GPR, file, I_or_D) trying to add
                operand_data = ((operand_data) | (RAM[address_location+1] << 0x8));//joins the data with the lower 8 bits
			}
			break;
		}
		case regI: {	//Index Modes modeid:6
			//PC-relative Mode 67
			if(curr_Register == PC) {
				address_op = (0xffff & (address_op + prog_cntr));//adds the PC to the address we are currently on to give us memory location
				operand_data = RAM[address_op];//takes the upper 8 bits of data from RAM]
				operand_data = (operand_data | (RAM[address_op+1] << 0x8));//joins the data with the lower 8 bits giving the address
			}
			else {
				operand_data = RAM[(0xffff & (address_op + GPR[curr_Register]))];//Read_mem(RAM, GPR, file, I_or_D) trying to add
                operand_data = ((operand_data) | (RAM[(0xffff &(address_op + GPR[curr_Register]))+1] << 0x8));//joins the data with the lower 8 bits
			}
			break;
		}
		case regADD: {	//Auto-Decrement Deferred Mode modeid:5
            if(curr_Register == SP)//Stack pops the stack by 2
            {
                GPR[curr_Register] -= 2;//decrement the stack pointer by 2 (push)
                operand_data = GPR[curr_Register];
            }
            else
            {
                GPR[curr_Register] -= 2;		//Decrement before dereferencing
                operand_data = RAM[GPR[curr_Register]];
                operand_data = ((operand_data) | (RAM[GPR[curr_Register]+1] << 0x8));
                operand_data = RAM[operand_data];
                operand_data = ((operand_data) | (RAM[operand_data+1] << 0x8));
            }
			break;
		}
		case regAD: {	//Auto-Decrement Mode mode: 4

			GPR[curr_Register] -= 2;		//Decrement before dereferencing
			operand_data = RAM[GPR[curr_Register]];
			operand_data = ((operand_data) | (RAM[GPR[curr_Register]+1] << 0x8));
			break;
		}
		case regAID: {	//Auto-Increment Deferred Modes modeid:3
            if(curr_Register == SP)//Stack pops the stack by 2
            {
                operand_data = GPR[curr_Register];
                GPR[curr_Register] += 2;//increment the stack pointer by 2 (pop)
            }
			//PC-relative Absolute Mode
			else if(curr_Register == PC)
            {
                operand_data = RAM[address_op];//Set a value stored in memory
                operand_data = ((operand_data) | (RAM[GPR[curr_Register]+1] << 0x8));
            }
			else
			{
				operand_data = RAM[GPR[curr_Register]];
				operand_data = ((operand_data) | (RAM[GPR[curr_Register]+1] << 0x8));
				operand_data = RAM[operand_data];// Address of the address
				operand_data = ((operand_data) | (RAM[operand_data+1] << 0x8));
				GPR[curr_Register] += 2;		//Increment after dereferencing
			}
			break;
		}
		case regAI: {	//Register Auto-Increment Modes mode:2

			//PC-relative Immediate Addressing Mode
			if(curr_Register == PC)
				operand_data = address_op;
			else
			{
				operand_data = RAM[GPR[curr_Register]];//fetches the address in the register and then increments by one
				operand_data = ((operand_data) | (RAM[GPR[curr_Register]+1] << 0x8));
				GPR[curr_Register] += 2;
			}
			break;
		}
		case regD: {	//Register Deferred Mode mode: 1
			operand_data = RAM[GPR[curr_Register]];//Goes to the RAM address stored in the register location
			operand_data = ((operand_data) | (RAM[GPR[curr_Register]+1] << 0x8));//Goes to the RAM address stored in the register location
			break;
		}
		case regS: {	//Register Mode mode: 0
			operand_data = GPR[curr_Register];	//Read value in specified register
			break;
		}
		default: {		//Invalid Addressing Mode

			cout << "Invalid Addressing Mode\n";
			break;
		}
	}
    return operand_data;
}

/**************************************************************
This function sets the status flags based on an operation



**************************************************************/
void StatusFlags(PSW & Status_word,int regDest, int ignore)
{
  //if the result of the register is zero then set the flag otherwise set to false
  if(regDest == 0)//setting the zero flag on the Status flags
  {
    Status_word.Z = true;
  }
  else
  {
    Status_word.Z = false;
  }

  //if the result is a negative number
  if(regDest < 0) //setting the negative flag on the Status flags
  {
    Status_word.N = true;
  }
  else
  {
    Status_word.N = false;
  }

  //if the result doesn't equal a 16 bit equivalent then overflow
  if(regDest < -32768 || regDest > 32767 )
  {
    Status_word.V = true;
  }
  else
  {
    Status_word.V = false;
  }

  if(ignore == 1)//used to ignore carry modification
   {
      //check for a carry
      if(((regDest) & (1 << 15)) || (((regDest < 0) && (((regDest) & (1 << 15)) != 0))))
      {
        Status_word.C = true;
      }
      else
      {
        Status_word.C = false;
      }
   }

}


/**************************************************************
This function updates the Program Counter to the Branch Target
address.


**************************************************************/
void TakeBranch(int & Program_Counter, int & branch_offset) {

	//Add the offset(word offset) to PC
	Program_Counter = Program_Counter + (2 * branch_offset);

	return;
}
