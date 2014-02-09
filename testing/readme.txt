Hey Guys:

  To use the test script:

    1) Make sure you have the entire testing folder pulled
    2) Create output directories:
         Our program cannot create folders to dump files into, so the output paths must be manually created
         before running the test. Your /testing/output directory should contain the following subdirectories:
            ConditionalBranches
            ConditionCode
            DoubleOperand
            Other
            SingleOperand
            System
            AddressingModes
            SubroutineCall
    3) Open "config.ini"
    4) Look for the "executable =" line in the [PATHS] section (line 29)
    5) After the equals sign, enter the complete absolute path to the executable your compiler outputs
    6) Run the script: "python3 ./validate.py"
    
  Possible Problems:

    If you get an error that says: "ImportError: No module named configparser" you're running the script under an 
    old version of Python. If you have Python 3.3 installed, try running it from the command line with the
    command "python3 validate.py" (Windows) or "python3 ./validate.py" (Linux) to force it to use your Python 3
    installation.

    No symbolic links are allowed in the executable path.

    The script will COPY your executable from the path you name into the testing directory and execute from there.
    It will do this every time you run the script, so you should be able to set the path once in the config file
    and forget about it, but remember to update it if you change where your compiler places its output.
    

