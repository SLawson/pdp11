# File:     set_address.py
# Author:   Scott Lawson
# Version:  1.0
# Date:     2/3/2014
# 
# This file opens all of the *.lst files in the current working directory, and finds the address
# that follows the START: label. It then finds the corresponding *.ascii file (they must be the 
# same name) and puts that address at the top of the file, prepended with a "*".

import glob
import sys

def main():

  for fi_name in glob.glob("*.lst"):

    with open(fi_name) as fi:
      lines = fi.readlines()
      for n, line in enumerate(lines):
        k = 1
        found = False
        # iterate over lines searching for START:
        if "START:" in line:
          while True:
            try:
              addr = lines[n+k].split()[1]
              found = True
              break

            except IndexError:
              # catching exception with there was are blank lines between START and the first instruction
              k += 1
              if k > 99:
                print("\nError: No non-blank lines found after START: label.")
                sys.exit(1)

          if found:
            break

      else:
        print("\nError: No START: label found in {}.".format(fi_name))

      try:
        ascii_fi = "{}.ascii".format(fi_name.split('.')[0])
        with open(ascii_fi) as out_fi:
          new_lines = out_fi.readlines()

        new_lines.insert(0, "*{}\n".format(addr))

        with open(ascii_fi, 'w') as out_fi:
          out_fi.writelines(new_lines)

      except FileNotFoundError:
        print("Error: could not open ascii file:\n{}".format(ascii_fi))

if __name__ == "__main__":
  main()

