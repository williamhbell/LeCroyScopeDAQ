The main.cxx runs either one of 

LeCroyScopeDaq // Reads the binary data from the scope and decodes it into ROOT form.
LeCroyScopeDaq // Reads the binary data from the scope and writes it straight to a file.
LeCroyDataProcessor // Reads a binary file and converts it to a ROOT file.

The fourth argument passed into the LeCroyScopeDaq constructor controls if the data are converted into ROOT format or not.  If the value is false then the binary data are just written to file.

---

ApplicationBase
---------------
Contains a set of pointers to service classes.  The classes LeCroyScopeDaq and LeCroyDataProcessor inherit from this class.

BinaryIO
--------
A class to write a binary trace from memory into a file or read the trace back from file into memory.

IOmethods
---------
A collection of methods for decoding files and writing file names.

Keyboard
--------
This class provides the functionality to run until the character Q is pressed. kbhit is not available in LINUX and therefore this class provides this functionality.

LeCroyDsoBinaryDecoder
----------------------
Contains functions to decode the different binary data types encoded in a scope trace.  See the last section of the remote operation LeCroy manual for comparison.

LeCroyDsoTimeStamp
------------------
Is a class to contain the values found in a trigger time stamp returned from the scope.

LeCroyDsoTrace
--------------
A class to contain one full trace decoded from binary.  In any readout there would be one of these objects per enabled trace.

LeCroyScope
-----------
This is the control centre for setting up and reading traces from the scope.

lecroy_tcp
----------
Freeware TCP routines for sending and receiving information over the network to the scope.

LeCroyTraceDecoder
------------------
This class is the control centre of trace decoding.  It calls the LeCroyDsoBinaryDecoder methods to decode a binary trace.

RootTreeOutput
--------------
A class to provide ROOT TTree output.  This is a simple class which just copies the values from LeCroyDsoTrace into a TTree.

ScopeSettings
-------------
Reads the program settings from the text file control/settings.txt and provides methods to access these settings.


-----------------

To use some other output format it should be fairly straight forward to write a class similar to RootTreeOutput.  Then if you don't want to bother with ROOT remove RootTreeOutput from the OBJECTS= list in the Makefile.
