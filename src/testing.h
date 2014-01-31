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
#include "pdp11.h"


extern string *reg_file;

int reg_dump(int GPR[], PSW Status_word); // dumps register values to a text file

#endif // TESTING_H_
