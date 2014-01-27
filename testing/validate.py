# ************************************************************************************************
# File:           validate.py
# Author:         Scott Lawson
# Version:        1.0
# Date:           1/26/2014
# Python Version: 3.3
#
# The purpose of this script is to run a program multiple times with different input parameters
# and compare the output files generated.
#
# It requires two files to coordinate the testing:
#   control.ini   Provides paths and test parameters
#   map.txt       Maps input files to their expected outputs
#
# This script should reside in a folder that also contains the following three folders:
#   input         The target program's input files
#   model         Example output files that the real output will be compared against
#   output        Location to store the real output files
#
# Test results will be printed to stdout and logged in the file test_log.txt. See "control.ini"
# and "map.ini" for details on those files' expected formats.
# ************************************************************************************************

import os
import sys
import shlex
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
    subprocess.call("{}".format(self._inv), shell=True)

    for file_pair in self._file_list:
      try:
        if not filecmp.cmp(file_pair[0], file_pair[1], shallow=False):
          result = 'Fail'
      except FileNotFoundError as e:
          print(e)
          result = 'Fail: One of the following output files was not found:\n\t\t\t{}\n\t\t\t{}'.format(file_pair[0], file_pair[1])
        
    return result



class TestSuite(object):
  def __init__(self, program, out_dir, model_dir, invocations, flags, files):
    self._program = os.path.join(os.getcwd(), program)
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
      
      # now parse the invocation to extract file names for comparisons
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

      self._cases.append(TestCase(case_id, inv, file_list))

    return

  def execute(self):
    summary = 0
    results = ["\nProgram Under Test: {}"
               "\nTime: {}\n"
               "\nTest ID\t\tResult"
               .format(self._program, datetime.datetime.now())]

    for test_case in self._cases:
      temp_result = test_case.execute()
      results.append("{}\t\t{}".format(test_case.test_id, temp_result))

      if temp_result != "Pass":
        summary = 1
    
    if summary:
      results.append("\n\nTest Summary: Fail\n\n")
    else:
      results.append("\n\nTest Summary: Pass\n\n")

    return "\n".join(results)



def main():

  # get information from config file
  config = configparser.ConfigParser()
  config.read('config.ini')
  
  try:
    program = config['PATHS']['name']
    out_dir = config['PATHS']['out_path']
    model_dir = config['PATHS']['model_path']

  except KeyError:
    print("\nValidation Error: The file 'config.ini' must exist and it must contain the section "
          "'[PATHS]'. This section must define 'name', 'out_path', and 'model_path'.\n")
    sys.exit(1)

  suite = TestSuite(program, out_dir, model_dir, config['INVOCATIONS'], config['FLAGS']['flags'], config['FILES'])
  result_string = suite.execute()

  print(result_string)
  with open('test_log.txt', 'a') as fi:
    fi.write(result_string)

  sys.exit(0)



if __name__ == "__main__":
  main()

