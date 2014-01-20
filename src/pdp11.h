//******************************************************************************
// pdp11.h
//
//  Created on: Jan 20, 2014
//      Author: Rob Gaskell
// 	   Project: pdp11
//******************************************************************************

#ifndef PDP11_H_
#define PDP11_H_

#include "stdbool.h"
#include <iostream>
using namespace std;

//Architecture parameters
#define MEM_SIZE        65536   //Mem size in bytes
#define REGISTERS       8       //Number of GPR's

//OpCode Mnemonics
//TODO Brennttt use #define's here

//General Purpose Register names
#define R0      0
#define R1      1
#define R2      2
#define R3      3
#define R4      4
#define R5      5
#define SP      6
#define PC      7

//Address Mode Mnemonics
//TODO Jordan use #define's here

//Decoded instruction and destination address
struct instruction {
  int opcode;           //Instruction OpCode
  int operand1;         //Fetched Operand1
  int operand2;         //Fetched Operand2
  int result;           //Result of Operation
  int write_flag;       //Write-result-to-RAM flag

  int dest_addr;        //Memory write address
  int dest_mode;        //Addressing mode
};

//Processor Status Word fields
struct PSW {
  int priority;
  bool T;
  bool N;
  bool Z;
  bool V;
  bool C;
};

#endif//PDP11_H_
