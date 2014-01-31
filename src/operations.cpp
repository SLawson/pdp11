//******************************************************************************
// operations.cpp
//
//  Created on: Jan 20, 2014
//      Author: Rob Gaskell		PSU ID# 914464617
// 	   Project: pdp11
//******************************************************************************

#include "pdp11.h"
#include "operations.h"

int Operation(instruction & current_inst, int GPR [], PSW & Status_word) {

  //TODO Rob/Brett implement instruction ops, populate current_inst.result

        int returnflag = 0;

        if(current_inst.instSel == DOUBLE_OP)
        {
            switch(current_inst.opcode){

                case MOV: //dest = src
                {
                    GPR[current_inst.destination] = GPR[current_inst.source];
                    StatusFlags(Status_word,GPR[current_inst.destination],0,1);
                    Status_word.V = false;//sets the overflow flag to false
                    returnflag = 1; //tells the program to return the modified register
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
                default:
                {
                    cout << "error in double op\n";
                }

        }
        }
        else if(current_inst.instSel == SINGLE_OP)
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

                    break;
                }
                case ROL:
                {

                    break;
                }
                case ASR:
                {

                   break;
                }
                case ASL:
                {

                    break;
                }
                case JMP:
                {


                    break;
                }
                case SWAB:
                {

                    break;
                }
                default:
                {
                    cout << "error in single op\n";
                }
            }
        }



if(returnflag == 1)//returns the destination
{
   return GPR[current_inst.dest_addr];
}

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
