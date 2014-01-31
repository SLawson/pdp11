//******************************************************************************
// testing.cpp
//
//  Created on: Jan 26, 2014
//      Author: Scott Lawson
// 	   Project: pdp11
//******************************************************************************

#include "testing.h"

string* reg_file = NULL; // name if register dump file - set in initialization function

int reg_dump(int GPR[], PSW Status_word){

  ofstream fi;                     // output file stream
  int status = 0;                  // return code

  if (reg_file != NULL){

    fi.open((*(reg_file)).c_str());

    if (fi.is_open()){
      for (int i=0; i<REGISTERS; ++i){
        fi << "R" << i << "," << GPR[i] << endl;
      }
      // dump PSW also
      fi << Status_word.priority << Status_word.T << Status_word.N << Status_word.Z
         << Status_word.V << Status_word.C << endl;
    }

    // handle file that couldn't be opened
    else{
      cout << "\nError: Register dump failed." << endl;
      status = 1;
    }
    
    fi.close();
    delete reg_file;
  }

  return (status);
}

