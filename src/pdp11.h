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
#include <fstream>
using namespace std;

//Debug configuration flag
#ifdef _DEBUG
#define DEBUG_FLAG      true
#else
#define DEBUG_FLAG      false
#endif

//Architecture parameters
#define MEM_SIZE        65536   //Mem size in bytes
#define REGISTERS       8       //Number of GPR's

//OpCode Mnemonics
//TODO Brennttt use #define's here lol nice rob hahahah

//Double-operand instructions (DOUBLE_OP)
//byteSel = 0
#define MOV     01 //dest = src
#define CMP     02 //compute src - dest, set flags only
#define BIT     03 //compute dest & src set flags only
#define BIC     04 //dest &= ~src
#define BIS     05 //Logical OR dest|=src
#define ADD     06 //dest +=src
//byteSel = 1
#define MOVB    01 //dest = src
#define CMPB    02 //compute src - dest, set flags only
#define BITB    03 //compute dest & src set flags only
#define BICB    04 //dest &= ~src
#define BISB    05 //Logical OR dest|=src
#define SUB     06 //dest -=src

//Misc
#define Jump    004r//Jumps to subroutine
#define Emu     104x//Emulator trap

//Single-operand instructions (SINGLE_OP)
//byteSel = 0
#define SWAB    003     //Swap Bytes
#define CLR     050     //Clear: dest = 0
#define COM     051     //Complement dest = ~dest
#define INC     052     //Increment: dest +=1
#define DEC     053     //Decrement: dest -=1
#define NEG     054     //Negate: dest = −dest
#define ADC     055     //Add carry: dest += C
#define SBC     056     //Subtract carry: dest −= C
#define TST     057     //Test: Load src, set flags only
#define ROR 	060     //Rotate right 1 bit
#define ROL	061     //Rotate left 1 bit
#define ASR	062     //Shift right: dest >>= 1
#define ASL	063     //Shift left: dest <<= 1
#define MARK    064     //Return from subroutine, skip 0..63 instruction words
#define MFPI	065     //Move from previous I space: −(SP) = src
#define MTPI	066     //Move to previous I space: dest = (SP)+
#define SXT	067     //Sign extend: dest = (16 copies of N flag)
//byteSel = 1
#define CLRB    050     //Clear: dest = 0
#define COMB    051     //Complement dest = ~dest
#define INCB    052     //Increment:dest +=1
#define DECB    053     //Decrement: dest -=1
#define NEGB    054     //Negate: dest = −dest
#define ADCB    055     //Add carry: dest += C
#define SBCB    056     //Subtract carry: dest −= C
#define TSTB    057     //Test: Load src, set flags only
#define RORB    060     //Rotate right 1 bit
#define ROLB    061     //Rotate left 1 bit
#define ASRB    062     //Shift right: dest >>= 1
#define ASLB    063     //Shift left: dest <<= 1
#define MTPS	064     //Move to status: PS = src
#define MFPD	065     //Move from previous D space: −(SP) = src
#define MTPD	066     //Move to previous D space: dest = (SP)+
#define MFPS	067     //Move from status: dest = PS

//Conditional branch instructions (CONDITIONAL_OP)
//byteSel = 0
#define BR              01      //Branch unconditionally
#define BNE             02      //Branch if not equal (Z=0)
#define BEQ             03      //Branch if equal (Z=1)
#define BGE             04      //Branch if greater that or equal (N|V = 0)
#define BLT             05      //Branch if less than (N|V = 1)
#define BGT             06      //Branch if greater than (N^V = 1)
#define BLE             07      //Branch if less than or equal (N^V = 0)
//byteSel = 1
#define BPL             00      //Branch if plus (N=0)
#define BMI             01      //Branch if minus (N=1)
#define BHI             02      //Branch if higher than (C|Z = 0)
#define BLOS            0101    //Branch if lower or same (C|Z = 1)
#define BVC             04      //Branch if overflow clear (V=0)
#define BVS	        05      //Branch if overflow set (V=1)
#define BCC_BHIS        06      //Branch if carry clear (C=0)
#define BCS_BLO         07      //Branch if carry set (C=1)

//Condition Code uctions (COND_CODE_OP)
//byteSel = 0
#define CLC     0241    //Clear C (Carry)
#define CLV     0242    //Clear V (Overflow)
#define CLZ     0244    //Clear Z (Zero)
#define CLN     0250    //Clear N (Negative)
#define SEC     0261    //Set C
#define SEV     0262    //Set V
#define SEZ     0264    //Set Z
#define SEN     0270    //Set N

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

//General register addressing modes
#define regS      	0 //register
#define regD     	1 //register deferred
#define regAI	  	2 //auto increment
#define regAID	 	3 //auto increment deferred
#define regAD	  	4 //auto decrement
#define regADD	 	5 //auto decrement deferred
#define regI    	6 //index
#define regID   	7 //index deferred

//Program counter addressing modes
#define pcImm		27 //immediate
#define pcAbs		37 //absolute
#define pcRel		67 //relative
#define pcRelD		77 //relative deferred

//Stack addressing modes
#define stackD      	16 //deferred
#define stackAI     	26 //auto increment
#define stackAID  	36 //auto increment deferred
#define stackAD	 	46 //auto decrement
#define stackI	  	66 //indexed
#define stackID	 	76 //indexed deferred

//instruction type categories
#define JUMP							0
#define SINGLE_OP					1
#define DOUBLE_OP					2
#define DOUBLE_OP_SP			3
#define CONDITIONAL_OP		4
#define COND_CODE_OP    5

//Decoded instruction and destination address
struct instruction {
  int instSel;		//Identify the instruction type

  int byteSel;		//For byte instructions
  int opcode;           //Instruction OpCode
  int source;           //Fetched Operand1
  int destination;      //Fetched Operand2
  int modeSrc;		//Addressing mode of the src
  int modeDest;		//Addressing mode of the destination
  int offset;
  int regster;
		
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
