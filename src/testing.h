//******************************************************************************
// testing.h
//
//  Created on: Jan 26, 2014
//      Author: Scott Lawson
// 	   Project: pdp11
//******************************************************************************

#ifndef TESTING_H_
#define TESTING_H_

#include <fstream>
#include <iomanip>
#include "pdp11.h"


extern string *reg_file;
extern bool display;
extern bool dump_flag;
extern bool mem_tracker[MEM_SIZE];

int reg_dump(bool file_out, bool std_out); // dumps register values to a text file
void mem_dump();

#endif // TESTING_H_
