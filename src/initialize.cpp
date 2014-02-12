//******************************************************************************
// initialize.cpp
//
//  Created on: Jan 20, 2014
//      Author: Scott Lawson
// 	   Project: pdp11
//******************************************************************************

#include "initialize.h"

int initialize(int RAM [], int argc, char * argv[], int* PC_Reg, string &out_file) {

  // misc vars
  int status = 0;                  // status of initialization operations
  int load_address = 0;            // pointer into RAM for storing file data
  int address_flag = 0;            // indicates whether or not the file has specified a starting load address
  int i = 1;                       // loop control variable
  string temp;                     // temporary string storage during file parsing
  command result;                  // stores command from parsed line
  ifstream fi;                     // input file stream

  // default values if user doesn't provide them
  *PC_Reg = 010;                   // default PC if none specified
  string fi_name("default.ascii"); // default input file name
  // out_file default name is set in main when constructed

  // option strings
  string fi_opt("-f");             // option for specifying file name
  string pc_opt("-c");             // option for specifying initial program counter
  string out_fi("-o");             // option for specifying output trace file name/directory
  string reg_fi("-r");             // option for specifying register dump file name/directory
  string disp_en("-d");             // option for enabling register prints

  while (i < argc){

    if(!fi_opt.compare(argv[i])){
      fi_name.assign(argv[i+1]);
      i += 1;
    }

    else if(!out_fi.compare(argv[i])){
      out_file.assign(argv[i+1]);
      i += 1;
    }

    else if(!disp_en.compare(argv[i])){
      display = 1;
      i += 1;
    }

    else if(!reg_fi.compare(argv[i])){
      reg_file = new string(argv[i+1]);
      i += 1;
    }

    else if(!pc_opt.compare(argv[i])){
      *PC_Reg = (int) strtol(argv[i+1], NULL, 8);
      address_flag = 1;
      i += 1;
    }

    else{
      cout << "\n\nError: Invalid option specified on the command line: " << argv[i]
           << endl;
      status = 1;
    }

    ++i;
  }

  if (status){
    return (status);
  }

  fi.open(fi_name.c_str());

  if (fi.is_open()){
    i = 1;
    while (getline(fi, temp) && !status){
      // parse the line into command and value
      result = parse_line(temp);

      if (result.value >> 16){
        cout << "\nError: Value provided on line " << i << " of input file is greater "
             << "than 16 bits in length." << endl;
        status = 1;
        break;
      }

      switch (result.action){

        case '-': // storing value at load address

          if ((load_address >= MEM_SIZE) || (load_address < 0)){
            cout << "Error: Command on line " << i << " of input file would cause a value "
                 << "to be loaded at an address outside of the memory array.\nAddress: "
                 << load_address << endl;
            status = 1;
          }

          else{
            RAM[load_address] = result.value & 0x00FF;
            RAM[load_address+1] = (result.value & 0xFF00) >> 8;
            load_address += 2;
          }
          break;

        case '@': // changing load address
          load_address = result.value;
          break;

        case '*': // setting intitial PC value
          if (address_flag){
            cout << "\nWarning: Starting PC value has been specified a second time "
                 << "on line " << i << endl;
          }

          else{
            *PC_Reg = result.value;
            address_flag = 1;
          }

          break;

        default: // unrecognized command symbol
          cout << "\nError: Invalid command symbol found on line " << i
               << " of input file." << endl;
          status = 1;
      }

      ++i;
    }

    // confirm that EOF was reached
    if (!fi.eof()){
      cout << "Error: An unknown erorr occured before the file parsing completed." << endl;
      status = 1;
    }
  }

  // handle file that couldn't be opened
  else{
    cout << "\nError: Could not open input file.\nFile name: " << fi_name << endl;
    status = 1;
  }

  fi.close();
  if(fi.is_open()){
    cout << "\nWarning: Input file " << fi_name << " did not close properly." << endl;
  }

  return (status);
}

// function for splitting and formatting each line of the input file
command parse_line(string line_in){
  command result;             // return container
  char* buffer;               // character buffer to convert the octal value properly
  int i = 0;                  // loop control variable
  int len = line_in.length(); // length of input string in characters (space for string
                              // terminator is not added because the buffer will not contain
                              // the command symbol that prepends each line)

  result.action = line_in[0]; // retrieve command character

  buffer = new char[len];

  for(i=0; i<len; ++i){
    buffer[i] = line_in[i+1];
  }
  buffer[i+1] = '\0'; // append string terminator

  result.value = (int) strtol(buffer, NULL, 8); // translate and store octal value

  delete [] buffer;

  return result;
}

