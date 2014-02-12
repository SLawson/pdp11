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

int reg_dump(int GPR[], PSW* Status_word, bool file_out, bool std_out); // dumps register values to a text file
void status_dump(int GPR[], PSW* Status_word); // dumps register values to the screen in octal

#endif // TESTING_H_
