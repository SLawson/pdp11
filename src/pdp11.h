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
#define DEBUG_FLAG		true
#else
#define DEBUG_FLAG		false
#endif

//Architecture parameters
#define MEM_SIZE		65536	//Mem size in bytes
#define REGISTERS		8		//Number of GPR's

//Decoded instruction and destination address
struct instruction {
	int instSel;		//Identify the instruction type
	int byteSel;		//For byte instructions
	int opcode;			//Instruction OpCode

	int source;			//Source Address
	int sourceReg;		//Source Operand Register
	int modeSrc;		//Addressing mode of the src
	int src_addr;       //Source memory location
	int srcPC;

	int destination;	//Destination Address
	int destReg;		//Destination Operand Register
	int modeDest;		//Addressing mode of the destination
	int dest_addr;		//Memory write address
	int destPC;

	int offset;			//Branch target PC-relative offset (# of words, signed)

	int result;			//Result of Operation
	int write_flag;		//Write-result-to-RAM flag
	bool Op_flag;		//Requires call to Operations() flag

};

//Processor Status Word
struct PSW {

	int priority;	//Interrupt Priority
	bool T;			//
	bool N;			//Negative Flag
	bool Z;			//Zero Flag
	bool V;			//Overflow Flag
	bool C;			//Carry Flag
};

//Global variables
extern int GPR[REGISTERS];	//General Purpose Registers
extern PSW Status_word;		//Current PSW
extern int RAM[MEM_SIZE];	//Contents of main memory
extern ofstream file;		// trace file stream

//Memory Access Options
#define NOPRINT	-1
#define READ		0
#define WRITE		1
#define FETCH		2

//OpCode Mnemonics

//Double-operand instructions (DOUBLE_OP)
//byteSel = 0
#define MOV		01	//dest = src
#define CMP		02	//compute src - dest, set flags only
#define BIT		03	//compute dest & src set flags only
#define BIC		04	//dest &= ~src
#define BIS		05	//Logical OR dest|=src
#define ADD   06	//dest +=src
//byteSel = 1
#define MOVB	01	//dest = src
#define CMPB	02	//compute src - dest, set flags only
#define BITB	03	//compute dest & src set flags only
#define BICB	04	//dest &= ~src
#define BISB	05	//Logical OR dest|=src
#define SUB		06	//dest -=src

//Single-operand instructions (SINGLE_OP)
//byteSel = 0
#define SWAB	3	//Swap Bytes
#define CLR		8	//Clear: dest = 0
#define COM		9	//Complement dest = ~dest
#define INC		10	//Increment: dest +=1
#define DEC		11	//Decrement: dest -=1
#define NEG		12	//Negate: dest = −dest
#define ADC		13	//Add carry: dest += C
#define SBC		14	//Subtract carry: dest −= C
#define TST		15	//Test: Load src, set flags only
#define ROR		16	//Rotate right 1 bit
#define ROL		17	//Rotate left 1 bit
#define ASR		18	//Shift right: dest >>= 1
#define ASL		19	//Shift left: dest <<= 1
#define MARK	20	//Return from subroutine, skip 0..63 instruction words
#define MFPI	21	//Move from previous I space: −(SP) = src
#define MTPI	22	//Move to previous I space: dest = (SP)+
#define SXT		23	//Sign extend: dest = (16 copies of N flag)
//byteSel = 1
#define CLRB	050	//Clear: dest = 0
#define COMB	051	//Complement dest = ~dest
#define INCB	052	//Increment:dest +=1
#define DECB	053	//Decrement: dest -=1
#define NEGB	054	//Negate: dest = −dest
#define ADCB	055	//Add carry: dest += C
#define SBCB	056	//Subtract carry: dest −= C
#define TSTB	057	//Test: Load src, set flags only
#define RORB	060	//Rotate right 1 bit
#define ROLB	061	//Rotate left 1 bit
#define ASRB	062	//Shift right: dest >>= 1
#define ASLB	063	//Shift left: dest <<= 1
#define MTPS	064	//Move to status: PS = src
#define MFPD	065	//Move from previous D space: −(SP) = src
#define MTPD	066	//Move to previous D space: dest = (SP)+
#define MFPS	067	//Move from status: dest = PS

//Conditional branch instructions (CONDITIONAL_OP)
//byteSel = 0
#define BR_JMP		01	//Branch unconditionally
#define BNE			02	//Branch if not equal (Z=0)
#define BEQ			03	//Branch if equal (Z=1)
#define BGE			04	//Branch if greater that or equal (N|V = 0)
#define BLT			05	//Branch if less than (N|V = 1)
#define BGT			06	//Branch if greater than (N^V = 1)
#define BLE			07	//Branch if less than or equal (N^V = 0)
//byteSel = 1
#define BPL			10	//Branch if plus (N=0)
#define BMI			01	//Branch if minus (N=1)
#define BHI			02	//Branch if higher than (C|Z = 0)
#define BLOS		03	//Branch if lower or same (C|Z = 1)
#define BVC			04	//Branch if overflow clear (V=0)
#define BVS			05	//Branch if overflow set (V=1)
#define BCC_BHIS	06	//Branch if carry clear (C=0)
#define BCS_BLO		07	//Branch if carry set (C=1)

//Subroutine instructions
#define JSR		04		//Jump to Subroutine
#define RTS		020		//Return from Subroutine

//General Purpose Register names
#define R0		0
#define R1		1
#define R2		2
#define R3		3
#define R4		4
#define R5		5
#define SP		6
#define PC		7

//Address Mode Mnemonics
//TODO Jordan use #define's here

//General register addressing modes
#define regS		00	//register
#define regD		01	//register deferred
#define regAI		02	//auto increment
#define regAID		03	//auto increment deferred
#define regAD		04	//auto decrement
#define regADD		05	//auto decrement deferred
#define regI		06	//index
#define regID		07	//index deferred

//instruction type categories
#define JUMP			0
#define SINGLE_OP		1
#define DOUBLE_OP		2
#define DOUBLE_OP_SP	3
#define CONDITIONAL_OP	4
#define COND_CODE_OP	5


#endif//PDP11_H_
