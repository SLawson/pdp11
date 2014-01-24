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
     doubleOpInst doubleOpInst = {0,0,0,0,0,0};

    switch(doubleOpInst.opCode){

        case MOV: //dest = src
        {
            GPR[doubleOpInst.destination] = GPR[doubleOpInst.source];
            Status_word = StatusFlags(Status_word,GPR[doubleOpInst.destination]);
        }
        case CMP://compute src - dest, set flags only
        {
            GPR[doubleOpInst.destination] =- GPR[doubleOpInst.source];
            Status_word = StatusFlags(Status_word,GPR[doubleOpInst.destination]);

        }
        case BIT://compute dest & src set flags only
        {
            GPR[doubleOpInst.destination]=GPR[doubleOpInst.destination] & GPR[doubleOpInst.source];
            Status_word = StatusFlags(Status_word,GPR[doubleOpInst.destination]);
        }
        case BIC://dest &= ~src
        {
            GPR[doubleOpInst.destination]=GPR[doubleOpInst.destination] & ~GPR[doubleOpInst.source];
            Status_word = StatusFlags(Status_word,GPR[doubleOpInst.destination]);

        }
        case BIS://Logical OR dest|=src
        {
            GPR[doubleOpInst.destination]=GPR[doubleOpInst.destination] |= GPR[doubleOpInst.source];
            Status_word = StatusFlags(Status_word,GPR[doubleOpInst.destination]);

        }
        case ADD://dest +=src
        {
            GPR[doubleOpInst.destination] += GPR[doubleOpInst.source];
            Status_word = StatusFlags(Status_word,GPR[doubleOpInst.destination]);
        }
        case SUB://dest -=src
        {
            GPR[doubleOpInst.destination] -= GPR[doubleOpInst.source];
            Status_word = StatusFlags(Status_word,GPR[doubleOpInst.destination]);
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

    return GPR[doubleOpInst.destination];
}

PSW StatusFlags(PSW & Status_word,int regDest)
{
    //write fancy code to take the result of the operation and set the status flags


}
