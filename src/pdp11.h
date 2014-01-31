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

//Double-operand instructions
#define MOV     01 //dest = src
#define CMP     02 //compute src - dest, set flags only
#define BIT     03 //compute dest & src set flags only
#define BIC     04 //dest &= ~src
#define BIS     05 //Logical OR dest|=src
#define ADD     06 //dest +=src
#define MOVB    11 //dest = src
#define CMPB    12 //compute src - dest, set flags only
#define BITB    13 //compute dest & src set flags only
#define BICB    14 //dest &= ~src
#define BISB    15 //Logical OR dest|=src
#define SUB     16 //dest -=src
#define MUL     070 //(R,R+1)= R*src
#define DIV     071 //(R,R+1) ÷ src; quotient in R, remainder in R+1
#define ASH     072 //Arithmetic shift: R <<= src
#define ASHC    073 //Arithmetic shift combined: (R,R+1) <<= src
#define XOR     074 //Exclusive OR: dest ^= reg
//op code 075 used for floating point
//op code 076 used for system instructions
#define SOB     077 //Subtract one branch: Decrement register

//Single-operand instructions
#define SWAB    0003//swap bytes
#define JMP     0001//Jumps to subroutine
#define Emu     104x//Emulator trap
#define CLR     0050//Clear: dest = 0
#define CLRB    1050//Clear: dest = 0
#define COM     0051//Complement dest = ~dest
#define COMB    1051//Complement dest = ~dest
#define INC     0052//Increment: dest +=1
#define INCB    1052//Increment:dest +=1
#define DEC     0053//Decrement: dest -=1
#define DECB    1053//Decrement: dest -=1
#define NEG     0054//Negate: dest = −dest
#define NEGB    1054//Negate: dest = −dest
#define ADC     0055//Add carry: dest += C
#define ADCB    1055//Add carry: dest += C
#define SBC     0056//Subtract carry: dest −= C
#define SBCB    1056//Subtract carry: dest −= C
#define TST     0057//Test: Load src, set flags only
#define TSTB    1057//Test: Load src, set flags only
#define ROR 	0060//Rotate right 1 bit
#define RORB    1060//Rotate right 1 bit
#define ROL	    0061//Rotate left 1 bit
#define ROLB    1061//Rotate left 1 bit
#define ASR	    0062//Shift right: dest >>= 1
#define ASRB    1062//Shift right: dest >>= 1
#define ASL	    0063//Shift left: dest <<= 1
#define ASLB    1063//Shift left: dest <<= 1
#define MARK    0064//Return from subroutine, skip 0..63 instruction words
#define MTPS	1064//Move to status: PS = src
#define MFPI	0065//Move from previous I space: −(SP) = src
#define MFPD	1065//Move from previous D space: −(SP) = src
#define MTPI	0066//Move to previous I space: dest = (SP)+
#define MTPD	1066//Move to previous D space: dest = (SP)+
#define SXT	    0067//Sign extend: dest = (16 copies of N flag)
#define MFPS	1067//Move from status: dest = PS

//Conditional branch instructions
#define BR      0004xx//Branch unconditionally
#define BNE     0010xx//Branch if not equal (Z=0)
#define BEQ     0014xx//Branch if equal (Z=1)
#define BGE     0020xx//Branch if greater that or equal (N|V = 0)
#define BLT     0024xx//Branch if less than (N|V = 1)
#define BGT     0030xx//Branch if greater than (N^V = 1)
#define BLE     0034xx//Branch if less than or equal (N^V = 0)
#define BPL     1000xx//Branch if plus (N=0)
#define BMI     1004xx//Branch if minus (N=1)
#define BHI     1010xx//Branch if higher than (C|Z = 0)
#define BLOS    1014xx//Branch if lower or same (C|Z = 1)
#define BVC     1020xx//Branch if overflow clear (V=0)
#define BVS	    1024xx//Branch if overflow set (V=1)
#define BCC     1030xx//Branch if carry clear (C=0)
#define BHIS    1030xx//Branch if higher or same (C=0)
#define BCS     1034xx//Branch if carry set (C=1)
#define BLO	    1034xx//Branch if lower than (C=1)





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

//instruction type mnemonics
#define JUMP							0
#define SINGLE_OP					1
#define DOUBLE_OP					2
#define DOUBLE_OP_SP			3
#define CONDITIONAL_OP		4

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
