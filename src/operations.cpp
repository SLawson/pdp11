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

int Operation(int RAM[],instruction & current_inst, int GPR [], PSW & Status_word) {

  //TODO Rob/Brett implement instruction ops, populate current_inst.result


  int returnflag = 0;
  int opsource;
  int opdestination;



  if(current_inst.instSel == DOUBLE_OP  && current_inst.byteSel == 0)
  {
    //Takes the destination address and fetches the correct value
    if(current_inst.modeDest == regID)//ID 7
        opdestination = RAM[current_inst.destination];//Read_mem(RAM, GPR, file, I_or_D) trying to add
    else if (current_inst.modeDest == regAI)//ID 2
        opdestination = current_inst.destination;
    //need to add the rest of the modes here for double op
    else
        cout << "not a valid mode\n";
    //Takes the source address and fetches the correct value or uses the source value
    if(current_inst.modeSrc == regID)
        opsource = RAM[current_inst.source];
    else if (current_inst.modeSrc == regAI)
        opdestination = current_inst.source;
    else
        cout << "not a valid mode for source\n";


    //once the correct destination and source the operation can occur
    switch(current_inst.opcode){

      case MOV: //dest = src
      {

        opdestination = opsource;
        StatusFlags(Status_word,opdestination,0,1);
        Status_word.V = false;//sets the overflow flag to false
        current_inst.result = opdestination;
        break;

      }
      case CMP://compute src - dest, set flags only
      {
        int regtemp;
        int16_t src16 = GPR[current_inst.source];
        int16_t dest16 = GPR[current_inst.destination];
        regtemp = GPR[current_inst.source] - GPR[current_inst.destination];
        dest16= src16 - dest16;
        StatusFlags(Status_word,regtemp,0,1);
        break;
      }
      case BIT://compute dest & src set flags only
      {
        GPR[current_inst.destination]=GPR[current_inst.destination] & GPR[current_inst.source];
        StatusFlags(Status_word,GPR[current_inst.destination],0,0);
        Status_word.V = false;//sets the overflow flag to false
        break;
      }
      case BIC://dest &= ~src
      {
        GPR[current_inst.destination]= ~GPR[current_inst.source] & GPR[current_inst.destination];
        StatusFlags(Status_word,GPR[current_inst.dest_addr],0,0);
        Status_word.V = false;//sets the overflow flag to false
        returnflag = 1;//sets to return
        break;

      }
      case BIS://Logical OR dest|=src
      {
        GPR[current_inst.dest_addr]= GPR[current_inst.source] | GPR[current_inst.destination];
        StatusFlags(Status_word,GPR[current_inst.dest_addr],0,0);
        Status_word.V = false;//sets the overflow flag to false
        returnflag = 1;//sets to return
        break;

      }
      case ADD://dest +=src
      {
        int16_t src16 = GPR[current_inst.source];
        int16_t dest16 = GPR[current_inst.destination];
        GPR[current_inst.destination] += GPR[current_inst.source];
        dest16 += src16;
        StatusFlags(Status_word,GPR[current_inst.destination],dest16,1);
        returnflag = 1;
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
	switch(current_inst.opcode){

		case SUB://dest -=src
		{
		int16_t src16 = GPR[current_inst.source];
		int16_t dest16 = GPR[current_inst.destination];
		GPR[current_inst.destination] -= GPR[current_inst.source];
		dest16 -= src16;
		StatusFlags(Status_word,GPR[current_inst.destination],dest16,1);
		returnflag = 1;
		break;
		}
	}
  }

  else if(current_inst.instSel == SINGLE_OP && current_inst.byteSel == 0)
  {
    switch(current_inst.opcode)
    {
      case CLR://set the destination to zero
      {
        GPR[current_inst.destination] = 0;
        Status_word.Z = true;//sets the zero flag to true
        Status_word.N = false;//sets the negative flag to false
        Status_word.C = false;//sets the carry flag to false
        Status_word.V = false;//sets the overflow flag to false
        returnflag = 1;
        break;
      }
      case COM://sets the destinations complement
      {
        GPR[current_inst.destination] = ~GPR[current_inst.destination];
        StatusFlags(Status_word,GPR[current_inst.destination],0,0);//just sets the zero and negative
        Status_word.C = false;//sets the carry flag to false
        Status_word.V = false;//sets the overflow flag to false
        returnflag = 1;
        break;
      }
      case INC://increments the destination value by one
      {
        int16_t dest16 = GPR[current_inst.destination];
        GPR[current_inst.destination] = GPR[current_inst.destination] + 1;
        dest16 = dest16 + 1;
        StatusFlags(Status_word,GPR[current_inst.destination],dest16,0);//just sets the zero and negative
        returnflag = 1;
        break;
      }
      case DEC://decrements the destination value by one
      {
        int16_t dest16 = GPR[current_inst.destination];
        GPR[current_inst.destination] = GPR[current_inst.destination] - 1;
        dest16 = dest16 - 1;
        StatusFlags(Status_word,GPR[current_inst.destination],dest16,0);//just sets the zero and negative
        returnflag = 1;
        break;
      }
      case NEG://changes the value to negative
      {
        int16_t dest16 = GPR[current_inst.destination];
        GPR[current_inst.destination] = ~GPR[current_inst.destination] + 1;
        dest16 = ~dest16 + 1;
        StatusFlags(Status_word,GPR[current_inst.destination],dest16,1);//just sets the zero and negative
        returnflag = 1;
        break;
      }
      case ADC:
      {
        int16_t dest16 = GPR[current_inst.destination];
        GPR[current_inst.destination] = GPR[current_inst.destination] + Status_word.C;
        dest16 = dest16 + Status_word.C;
        StatusFlags(Status_word,GPR[current_inst.destination],dest16,0);//just sets the zero and negative
        if(dest16 == -1)
        {
          Status_word.C = true;//sets the carry flag to true
        }
        returnflag = 1;
        break;
      }
      case SBC:
      {
        int16_t dest16 = GPR[current_inst.destination];
        GPR[current_inst.destination] = GPR[current_inst.destination] - Status_word.C;
        dest16 = dest16 - Status_word.C;
        StatusFlags(Status_word,GPR[current_inst.destination],dest16,0);//just sets the zero and negative
        if(dest16 == 0 && Status_word.C == true)
        {
            Status_word.C = false;//sets the carry flag to true
        }
        returnflag = 1;
        break;
      }
      case TST:
      {
        StatusFlags(Status_word,GPR[current_inst.destination],0,0);//just sets the zero and negative
        Status_word.C = false;//sets the carry flag to false
        Status_word.V = false;//sets the overflow flag to false
        break;
      }
      case ROR:
      {
        //load the 16 bit value
        int16_t dest16 = RAM[current_inst.destination]; 
        dest16 = (dest16 << 0x8) | RAM[current_inst.destination + 1];
        
        Status_word.C = (dest16 & 0x1);
        dest16 = (dest16 >> 0x1);
        dest16 = ((Status_word.C << 0xf) | dest16);
        
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
        current_inst.result = dest16;
        current_inst.write_flag = true;
        break;
      }
      case ROL:
      {
        //load the 16 bit value
        int16_t dest16 = RAM[current_inst.destination]; 
        dest16 = (dest16 << 0x8) | RAM[current_inst.destination + 1];
        
        Status_word.C = (dest16 & 0x8000);
        dest16 = (dest16 << 0x1);
        dest16 = (Status_word.C | dest16);
        
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
        current_inst.result = dest16;
        current_inst.write_flag = true;
        break;
      }
      case ASR:
      {
        //load the 16 bit value
        int16_t dest16 = RAM[current_inst.destination]; 
        dest16 = (dest16 << 0x8) | RAM[current_inst.destination + 1];
        
        Status_word.C = (dest16 & 0x1);
        dest16 = ((dest16 & 0X8000) | ((dest16 & 0x7fff) >> 0x1));
        
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
        current_inst.result = dest16;
        current_inst.write_flag = true;
        break;
      }
      case ASL:
      {
        //load the 16 bit value
        int16_t dest16 = RAM[current_inst.destination]; 
        dest16 = (dest16 << 0x8) | RAM[current_inst.destination + 1];
        
        Status_word.C = (dest16 & 0x8000);
        if (Status_word.C)
          dest16 = (0X8000 | ((dest16 & 0x7fff) << 0x1));
        else
          dest16 = ((dest16 & 0X7fff) & ((dest16 & 0x7fff) << 0x1));
        
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
        current_inst.result = dest16;
        current_inst.write_flag = true;
        break;
      }
      case SWAB:
      {
        //load the 16 bit value
        int16_t dest16 = RAM[current_inst.destination]; 
        dest16 = (dest16 << 0x8) | RAM[current_inst.destination + 1];
        
        dest16 = ((dest16 >> 0x8) | ((dest16 & 0xff) << 0x8));
        
        //set flags
        if ((dest16 & 0xff) == 0)
          Status_word.Z = true;
        else
          Status_word.Z = false;
          
        if ((dest16 & 0x80) == 0x80)
          Status_word.N = true;
        else
          Status_word.N = false;
          
        Status_word.C = false;
        Status_word.V = false;
        current_inst.result = dest16;
        current_inst.write_flag = true;
        break;
      }
      default:
      {
          cout << "error in single op\n";
          break;
      }
    }
  }

  //Conditional Branch Instructions
  //byteSel = 0: BR_JMP, BEQ, BNE, and Signed Conditional Branches
  else if(current_inst.instSel == CONDITIONAL_OP && current_inst.byteSel == 0) {
	switch(current_inst.opcode) {
		case BR_JMP: {			//Unconditional Branch/Jump

			GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			break;
		}
		case BNE: {				//Branch on Not Equal(Zero)

			if(Status_word.Z == false) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		case BEQ: {				//Branch on Equal(Zero)

			if(Status_word.Z == true) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		case BGE: {				//Branch on Greater than or Equal(Zero)

			if(Status_word.N == Status_word.V) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		case BLT: {				//Branch on Less Than(Zero)

			if(Status_word.N != Status_word.V) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		case BGT: {				//Branch on Greater Than(Zero)

			if((Status_word.N == Status_word.V) || Status_word.Z == false) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		case BLE: {				//Branch on on Less than or Equal(Zero)

			if((Status_word.N != Status_word.V) || Status_word.Z == true) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		default: {				//Invalid OpCode/byteSel values for CONDITIONAL_OP

		  cout << "Invalid Conditional Branch Instruction\n";
		  break;
		}
	}
  }
  //byteSel = 1: BMI, BPL, BCS, BCC, BVS, BVC, and Unsigned Conditional Branches
  else if(current_inst.instSel == CONDITIONAL_OP && current_inst.byteSel == 1) {
	switch(current_inst.opcode) {
		case BPL: {				//Branch on PLus

			if(Status_word.N == false) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		case BMI: {				//Branch on MInus

			if(Status_word.N == true) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		case BHI: {				//Branch on HIgher

			if(Status_word.C == false && Status_word.Z == false) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		case BLOS: {			//Branch on LOwer or Same

			if(Status_word.C == true || Status_word.Z == true) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		case BVC: {				//Branch on oVerflow Clear

			if(Status_word.V == false) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		case BVS: {				//Branch on oVerflow Set

			if(Status_word.V == true) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		case BCC_BHIS: {		//Branch on Carry Clear/Branch on HIgher or Same

			if(Status_word.C == false) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		case BCS_BLO: {			//Branch on Carry Set/Branch on LOwer

			if(Status_word.C == true) {
				GPR[PC] = GPR[PC] + (2 * current_inst.offset);
			}
			break;
		}
		default: {				//Invalid OpCode/byteSel values for CONDITIONAL_OP

		  cout << "Invalid Conditional Branch Instruction\n";
		  break;
		}
	}
  }

  //Condition Code Operator Instructions
  //Set or Clear Condition Codes (C,V,Z,N)
  else if(current_inst.instSel == COND_CODE_OP) {
	switch(current_inst.opcode) {
		case CLC: {		//Clear Carry Flag

			Status_word.C = false;
			break;
		}
		case CLV: {		//Clear Overflow Flag

			Status_word.V = false;
			break;
		}
		case CLZ: {		//Clear Zero Flag

			Status_word.Z = false;
			break;
		}
		case CLN: {		//Clear Negative Flag

			Status_word.N = false;
			break;
		}
		case SEC: {		//Set Carry Flag

			Status_word.C = true;
			break;
		}
		case SEV: {		//Set Overflow Flag

			Status_word.V = true;
			break;
		}
		case SEZ: {		//Set Zero Flag

			Status_word.Z = true;
			break;
		}
		case SEN: {		//Set Negative Flag

			Status_word.N = true;
			break;
		}
	}
  }
  /* -- this doesnt make sense
  if(returnflag == 1)//returns the destination
  {
     return GPR[current_inst.dest_addr];
  }*/

}

void StatusFlags(PSW & Status_word,int regDest, int16_t regDest16, int ignore)
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
  if(regDest == regDest16 )
  {
    Status_word.V = false;
  }
  else
  {
    Status_word.V = true;
  }

  if(ignore == 1)//used to ignore carry modification
   {
      //check for carry bit
      if(regDest >= 65536)
      {
        Status_word.C = true;
      }
      else
      {
        Status_word.C = false;
      }
   }

}
