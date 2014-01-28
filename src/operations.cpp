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

    //We need some way to decode the double operand


    switch(current_inst.opcode){

        case MOV: //dest = src
        {
            GPR[current_inst.dest_addr] = GPR[current_inst.modeSrc];
            Status_word = StatusFlags(Status_word,GPR[current_inst.dest_addr]);
        }
        case CMP://compute src - dest, set flags only
        {
            GPR[current_inst.dest_addr] =- GPR[current_inst.modeSrc];
            Status_word = StatusFlags(Status_word,GPR[current_inst.dest_addr]);

        }
        case BIT://compute dest & src set flags only
        {
            GPR[current_inst.dest_addr]=GPR[current_inst.dest_addr] & GPR[current_inst.modeSrc];
            Status_word = StatusFlags(Status_word,GPR[current_inst.dest_addr]);
        }
        case BIC://dest &= ~src
        {
            GPR[current_inst.dest_addr]=GPR[current_inst.dest_addr] & ~GPR[current_inst.modeSrc];
            Status_word = StatusFlags(Status_word,GPR[current_inst.dest_addr]);

        }
        case BIS://Logical OR dest|=src
        {
            GPR[current_inst.dest_addr]=GPR[current_inst.dest_addr] |= GPR[current_inst.modeSrc];
            Status_word = StatusFlags(Status_word,GPR[current_inst.dest_addr]);

        }
        case ADD://dest +=src
        {
            GPR[current_inst.dest_addr] += GPR[current_inst.modeSrc];
            Status_word = StatusFlags(Status_word,GPR[current_inst.dest_addr]);
        }
        case SUB://dest -=src
        {
            GPR[current_inst.dest_addr] -= GPR[current_inst.modeSrc];
            Status_word = StatusFlags(Status_word,GPR[current_inst.dest_addr]);
        }


//create a new function here
        case MUL://(R,R+1)= R*src
        {

        }
        case DIV://(R,R+1) ÷ src; quotient in R, remainder in R+1
        {

        }
        case ASH://Arithmetic shift: R <<= src
        {

        }
        case ASHC://Arithmetic shift combined: (R,R+1) <<= src
        {

        }
        case XOR://Exclusive OR: dest ^= reg
        {

        }
        case SOB://Subtract one branch: Decrement register
        {

        }
        default:
            cout << "error unknown instruction/n";
    }

    return GPR[current_inst.dest_addr];
}

PSW StatusFlags(PSW & Status_word,int regDest)
{
    //write fancy code to take the result of the operation and set the status flags


}
