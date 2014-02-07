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

    //Internal scratch pad registers
    int opsource;//this holds the value of the data (RAM or register)
    int opdestination;//this holds the value of the data (RAM or register)
    int regAddress = current_inst.destination;//

    opdestination = AddressmodesDecode(RAM, current_inst, GPR);

/*********************************************************************************
Double Operation

Take a source and destination memory location

*********************************************************************************/
  if(current_inst.instSel == DOUBLE_OP  && current_inst.byteSel == 0)
  {

    opsource = AddressmodesDecode(RAM, current_inst, GPR);

    //once the correct destination and source the operation can occur
    switch(current_inst.opcode){

      case MOV: //dest = src
      {

        opdestination = opsource;
        StatusFlags(Status_word,opdestination,1);
        Status_word.V = false;//sets the overflow flag to false
        break;

      }
      case CMP://compute src - dest, set flags only
      {
        int tempresult;
		tempresult = opsource - opdestination;
		StatusFlags(Status_word,tempresult,0);

        break;
      }
      case BIT://compute dest & src set flags only
      {
        opdestination=opdestination & opsource;
        StatusFlags(Status_word,opdestination,0);
        Status_word.V = false;//sets the overflow flag to false
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

  Only takes destination

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
        opdestination = ~opdestination;
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


/*******************************************************************
Write back to RAM or appropriate register
Only modifies the Destination works for both Double operand
and single operand

*******************************************************************/



    //return function here for the different modes
    if(current_inst.modeDest == regID)//ID 7 Read RAM data
    {
        current_inst.write_flag = true;
        current_inst.result = opdestination;
    }
    else if(current_inst.modeDest == regI)//ID6
    {
        current_inst.write_flag = true;
        current_inst.result = opdestination;
    }
    else if(current_inst.modeDest == regADD)//ID5
        opdestination;
    else if(current_inst.modeDest == regAD)//ID4
        opdestination;
    else if(current_inst.modeDest == regAID)//ID3
        opdestination;
    else if (current_inst.modeDest == regAI)//ID 2
        opdestination;
    else if(current_inst.modeDest == regD)//ID1
        GPR[regAddress] = opdestination;
    else if(current_inst.modeDest == regS)//ID0
        GPR[regAddress] = opdestination;
    //need to add the rest of the modes here for double op
    else
        cout << "not a valid mode\n";

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
		case C_NOP: {		//NOP version of clear COND_CODE_OP

			break;
		}
		case CLC: {			//Clear Carry Flag

			Status_word.C = false;
			break;
		}
		case CLV: {			//Clear Overflow Flag

			Status_word.V = false;
			break;
		}
		case CLC_V: {		//Clear Carry and Overflow Flag

			Status_word.C = false;
			Status_word.V = false;
			break;
		}
		case CLZ: {			//Clear Zero Flag

			Status_word.Z = false;
			break;
		}
		case CLC_Z: {		//Clear Carry and Zero Flag

			Status_word.C = false;
			Status_word.Z = false;
			break;
		}
		case CLV_Z: {		//Clear Overflow and Zero Flag

			Status_word.V = false;
			Status_word.Z = false;
			break;
		}
		case CLC_V_Z: {		//Clear Carry, Overflow and Zero Flag

			Status_word.C = false;
			Status_word.V = false;
			Status_word.Z = false;
			break;
		}
		case CLN: {			//Clear Negative Flag

			Status_word.N = false;
			break;
		}
		case CLC_N: {		//Clear Carry and Negative Flag

			Status_word.C = false;
			Status_word.N = false;
			break;
		}
		case CLV_N: {		//Clear Overflow and Negative Flag

			Status_word.V = false;
			Status_word.N = false;
			break;
		}
		case CLC_V_N: {		//Clear Carry, Overflow and Negative Flag

			Status_word.C = false;
			Status_word.V = false;
			Status_word.N = false;
			break;
		}
		case CLZ_N: {		//Clear Zero and Negative Flag

			Status_word.Z = false;
			Status_word.N = false;
			break;
		}
		case CLC_Z_N: {		//Clear Carry, Zero and Negative Flag

			Status_word.C = false;
			Status_word.Z = false;
			Status_word.N = false;
			break;
		}
		case CLV_Z_N: {		//Clear Overflow, Zero and Negative Flag

			Status_word.V = false;
			Status_word.Z = false;
			Status_word.N = false;
			break;
		}
		case CLC_V_Z_N: {	//Clear Carry, Overflow, Zero and Negative Flag

			Status_word.C = false;
			Status_word.V = false;
			Status_word.Z = false;
			Status_word.N = false;
			break;
		}
		case S_NOP: {		//NOP version of set COND_CODE_OP

			break;
		}
		case SEC: {			//Set Carry Flag

			Status_word.C = false;
			break;
		}
		case SEV: {			//Set Overflow Flag

			Status_word.V = false;
			break;
		}
		case SEC_V: {		//Set Carry and Overflow Flag

			Status_word.C = false;
			Status_word.V = false;
			break;
		}
		case SEZ: {			//Set Zero Flag

			Status_word.Z = false;
			break;
		}
		case SEC_Z: {		//Set Carry and Zero Flag

			Status_word.C = false;
			Status_word.Z = false;
			break;
		}
		case SEV_Z: {		//Set Overflow and Zero Flag

			Status_word.V = false;
			Status_word.Z = false;
			break;
		}
		case SEC_V_Z: {		//Set Carry, Overflow and Zero Flag

			Status_word.C = false;
			Status_word.V = false;
			Status_word.Z = false;
			break;
		}
		case SEN: {			//Set Negative Flag

			Status_word.N = false;
			break;
		}
		case SEC_N: {		//Set Carry and Negative Flag

			Status_word.C = false;
			Status_word.N = false;
			break;
		}
		case SEV_N: {		//Set Overflow and Negative Flag

			Status_word.V = false;
			Status_word.N = false;
			break;
		}
		case SEC_V_N: {		//Set Carry, Overflow and Negative Flag

			Status_word.C = false;
			Status_word.V = false;
			Status_word.N = false;
			break;
		}
		case SEZ_N: {		//Set Zero and Negative Flag

			Status_word.Z = false;
			Status_word.N = false;
			break;
		}
		case SEC_Z_N: {		//Set Carry, Zero and Negative Flag

			Status_word.C = false;
			Status_word.Z = false;
			Status_word.N = false;
			break;
		}
		case SEV_Z_N: {		//Set Overflow, Zero and Negative Flag

			Status_word.V = false;
			Status_word.Z = false;
			Status_word.N = false;
			break;
		}
		case SEC_V_Z_N: {	//Set Carry, Overflow, Zero and Negative Flag

			Status_word.C = false;
			Status_word.V = false;
			Status_word.Z = false;
			Status_word.N = false;
			break;
		}
	}
  }
}

/***************************************************************
Address modes handler



***************************************************************/

int AddressmodesDecode(int RAM[],instruction & current_inst, int GPR[]) {

int address_value;
int operand_data;

	//Takes the destination address and fetches the data from the RAM for the destination value
	switch (current_inst.modeDest) {
		case regID: {	//Index Deferred Modes

			//PC-relative Deferred Mode
			if(current_inst.regster == 7)
				operand_data = current_inst.destination;
			else {
				address_value = RAM[current_inst.destination];//Read_mem(RAM, GPR, file, I_or_D) trying to add
				operand_data = ((address_value << 0x8) | RAM[current_inst.destination+1]);
			}
			break;
		}
		case regI: {	//Index Modes

			//PC-relative Mode
			if(current_inst.regster == 7) {
				operand_data = current_inst.destination;
			}
			else {
				address_value = RAM[current_inst.destination];	//Read_mem(RAM, GPR, file, I_or_D) trying to add
				operand_data = ((address_value << 0x8) | RAM[current_inst.destination+1]);
			}
			break;
		}
		case regADD: {	//Auto-Decrement Deferred Mode

			GPR[current_inst.destination] -= 2;		//Decrement before dereferencing
			address_value = RAM[GPR[current_inst.destination]];
			address_value = ((address_value << 0x8) | RAM[GPR[current_inst.destination]+1]);
			address_value = RAM[address_value];
			operand_data = ((address_value << 0x8) | RAM[GPR[address_value]+1]);
			break;
		}
		case regAD: {	//Auto-Decrement Mode

			GPR[current_inst.destination] -= 2;		//Decrement before dereferencing
			address_value = RAM[GPR[current_inst.destination]];
			operand_data = ((address_value << 0x8) | RAM[GPR[current_inst.destination]+1]);
			break;
		}
		case regAID: {	//Auto-Increment Deferred Modes

			//PC-relative Absolute Mode
			if(current_inst.regster == 7)
				operand_data = current_inst.destination;
			else
			{
				address_value = RAM[GPR[current_inst.destination]];
				address_value = ((address_value << 0x8) | RAM[GPR[current_inst.destination]+1]);
				address_value = RAM[address_value];//
				operand_data = ((address_value << 0x8) | RAM[GPR[address_value]+1]);
				GPR[current_inst.destination] += 2;		//Increment after dereferencing
				return operand_data;
			}
			break;
		}
		case regAI: {	//Register Auto-Increment Modes

			//PC-relative Immediate Addressing Mode
			if(current_inst.regster == 7)
				operand_data = current_inst.destination;
			else
			{
				address_value = RAM[GPR[current_inst.destination]];//fetches the address in the register and then increments by one
				operand_data = ((address_value << 0x8) | RAM[GPR[current_inst.destination]+1]);
				GPR[current_inst.destination] += 2;
			}
			break;
		}
		case regD: {	//Register Deferred Mode
			address_value = RAM[GPR[current_inst.destination]];//Goes to the RAM address stored in the register location
			operand_data = ((address_value << 0x8) | RAM[GPR[current_inst.destination]+1]);//Goes to the RAM address stored in the register location
			break;
		}
		case regS: {	//Register Mode
			operand_data = GPR[current_inst.destination];	//Read value in specified register
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
    Status_word.V = false;
  }
  else
  {
    Status_word.V = true;
  }

  if(ignore == 1)//used to ignore carry modification
   {
      //check for a carry
      if((regDest & (1 << 15)) || ((regDest < 0 &&(regDest & (1 << 15) != 0))))
      {
        Status_word.C = true;
      }
      else
      {
        Status_word.C = false;
      }
   }

}
