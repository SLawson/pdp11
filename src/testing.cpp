//******************************************************************************
// testing.cpp
//
//  Created on: Jan 26, 2014
//      Author: Scott Lawson
// 	   Project: pdp11
//******************************************************************************

#include "testing.h"

string* reg_file = NULL; // name of register dump file - set in initialization function
bool display = 0;
bool dump_flag = 0;
bool mem_tracker[MEM_SIZE] = {false};

int reg_dump(bool file_out, bool std_out){

  ofstream fi;                     // output file stream
  int status = 0;                  // return code

  if (file_out){

    if (reg_file != NULL){

      fi.open((*(reg_file)).c_str());

      if (fi.is_open()){
        for (int i=0; i<REGISTERS; ++i){
          fi << "R" << i << "," << GPR[i] << endl;
        }
        // dump PSW also
        fi << Status_word.priority << Status_word.T << Status_word.Z
           << Status_word.N << Status_word.C << Status_word.V << endl;
      }

      // handle file that couldn't be opened
      else{
        cout << "\nError: Register dump to file failed." << endl;
        status = 1;
      }
      
      fi.close();
      delete reg_file;
    }
  }

  if (std_out){
      for (int i=0; i<REGISTERS; ++i){
        cout << "R" << i << "," << GPR[i] << endl;
      }
      // dump PSW also
      cout << Status_word.priority << Status_word.T << Status_word.Z
           << Status_word.N << Status_word.C << Status_word.V << endl;
  }

  return (status);
}

void mem_dump(){

  int i = 0;
  int temp = 0;
  
  cout << "\nAccessed Memory Locations and Current Data:\n" << endl;

  for (i=0; i<MEM_SIZE; i=i+2){
    if (mem_tracker[i]){
      temp = RAM[i] | (RAM[i+1] << 0x8);
      cout << setfill('0') << oct << setw(6) << i << "\t"
           << oct << setw(6) << temp << endl;
    }
  }

  return;
}

