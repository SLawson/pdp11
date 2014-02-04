# ************************************************************************************************
# File:           validate.py
# Author:         Scott Lawson
# Version:        1.3
# Date:           2/3/2014
# Python Version: 3.3
#
# The purpose of this script is to run a program multiple times with different input parameters
# and compare the output files generated. The file "config.ini" is required to run the test suite.
#
# This script is only intended for command-line based applications. It will copy the executable
# to the current working directory and repeatedly run the program with different command-line
# invocations specified in config.ini.
#
# Test results will be printed to stdout and logged in the file test_log.txt.
# ************************************************************************************************

import os
import sys
import shlex
import shutil
import filecmp
import datetime
import subprocess
import configparser

class TestCase(object):
  def __init__(self, ID, invocation, file_list):
    self.test_id = ID
    self._inv = invocation
    self._file_list = file_list
    return

  def execute(self):
    result = 'Pass'

    try:
      with open('newlines.txt') as newlines:
        subprocess.call("{}".format(self._inv), shell=True, stdin=newlines, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=4)
    except subprocess.TimeoutExpired:
      result = 'Fail: Program timed out'
      return result

    for file_pair in self._file_list:
      try:
        if not os.path.exists(file_pair[0]):
          result = 'Fail: The following output file was not found:\n\t\t\t{}'.format(file_pair[0])
          break

        elif not os.path.exists(file_pair[1]):
          result = 'Fail: The following output file was not found:\n\t\t\t{}'.format(file_pair[1])
          break

        elif not filecmp.cmp(file_pair[0], file_pair[1], shallow=False):
          result = 'Fail'
          break

      except FileNotFoundError as e:
          print(e)
          result = 'Fail: One of the following output files was not found:\n\t\t\t{}\n\t\t\t{}'.format(file_pair[0], file_pair[1])
        
    return result



class TestSuite(object):
  def __init__(self, program_path, program, out_dir, model_dir, invocations, flags, files):
    self._program = program
    self._cases = []
    flags = [flag.strip() for flag in flags.split(',')]
    
    # iterate over test cases
    for case_id in invocations:
      # first see if they have specific files listed
      try:
        file_list = [[os.path.join(out_dir, fi.strip()).strip(), os.path.join(model_dir, fi.strip()).strip()]
                     for fi in files[case_id].split(',')]
      except KeyError:
        file_list = []
      
      # now parse the invocations to extract file names for comparisons
      inv = invocations[case_id]
      parsed = shlex.split(inv)

      # iterate through provided flags...
      for opt in flags:

        # checking to see if it appears in the invocation
        if opt in parsed:
        
          # if the option appears, grab the argument immediately following it, which should be a file name
          name = parsed[parsed.index(opt)+1]
          # append to list of files for comparison
          file_list.append([name.strip(), os.path.join(model_dir, os.path.relpath(name,start=out_dir).strip())])

      # prepend argument list with program name
      inv = "{} {}".format(program_path, inv)
      self._cases.append(TestCase(case_id, inv, file_list))

    return

  def execute(self):
    summary = 0
    header = ["\nProgram Under Test: {}"
              "\nTime: {}\n"
              "\nTest ID\t\tResult"
              .format(self._program, datetime.datetime.now())][0]

    print(header)
    with open('test_log.txt', 'a') as fi:
      fi.write("{}\n".format(header))

    for test_case in self._cases:
      temp_result = test_case.execute()
      if temp_result != "Pass":
        summary = 1

      result_string = "{}\t\t{}".format(test_case.test_id, temp_result)

      print(result_string)
      with open('test_log.txt', 'a') as fi:
        fi.write("{}\n".format(result_string))

    
    if summary:
      summary_string = ("\n\nTest Summary: Fail\n\n")
    else:
      summary_string = ("\n\nTest Summary: Pass\n\n")

    print(summary_string)
    with open('test_log.txt', 'a') as fi:
      fi.write("{}\n".format(summary_string))

    return



def main():

  print("\n\nExecuting test suite...\n")

  # get information from config file
  config = configparser.ConfigParser()
  config.read('config.ini')
  
  try:
    program_path = config['PATHS']['executable']
    out_dir = config['PATHS']['out_path']
    model_dir = config['PATHS']['model_path']

  except KeyError:
    print("\nValidation Error: The file 'config.ini' must exist and it must contain the section "
          "'[PATHS]'. This section must define 'executable', 'out_path', and 'model_path'.\n")
    sys.exit(1)

  try:
    program = os.path.split(program_path)[1]
    if not program:
      print("\nProgram name not correctly parsed. Please confirm that there is a [PATHS] section in config.ini,"
            "and that it defines the variable \"executable\". \"Executable\" should specify the complete "
            "absolute path to the program to test, including the name of the program itself.")
      sys.exit(1)

    shutil.copyfile(program_path, os.path.join(os.getcwd(), program))
    shutil.copymode(program_path, os.path.join(os.getcwd(), program))

  except OSError:
    print("\nExecutable copy operation failed. Please confirm that you have write permissions to this directory.")

  suite = TestSuite(program_path, program, out_dir, model_dir, config['INVOCATIONS'], config['FLAGS']['flags'], config['FILES'])
  suite.execute()

  sys.exit(0)



if __name__ == "__main__":
  try:
    main()
  except KeyboardInterrupt:
    print("\n\nTest suite halted by CTRL-C.\n")

