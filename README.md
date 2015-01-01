LeCroyScopeDAQ
==============

Command line DAQ for LeCroy X-Stream oscilloscopes

Author: W. H. Bell, http://www.whbell.net/

Purpose
-------

General purpose program for acquiring scope traces from a LeCroy
X-Stream oscilloscope.

Description
-----------

The scope parameters are set from the values given in the
control/settings.txt file.  The scope is then setup and readout for a
given number of triggers.  The data taken from the scope are decoded
and recorded in a Root TTree saved in a root file.

------------------------------------------------------

Building the code
-----------------

```
cd src
make
cd ..
```

By default, the code requires ROOT [ http://root.cern.ch/ ] to build.
However, this dependency can be removed by removing the RootTreeOutput from the OBJECTS= list in the Makefile.

Setup the scope parameters
--------------------------

The scope settings are controlled by the text file

```
control/settings.txt
```

where each parameter name is given followed by at least one space or
tab, and then the value for the parameter.

Running the code
----------------

```
./bin/scopeDAQ.exe [options]
```

where [options] is optional and can be:
* -h to print the possible options
* -d to enable the debug mode, to print more information
* -m <value> to set the readout mode, where <value> can be 0, 1, or 2.
* -n <number of events> to set the executable to read a fixed number of events
* -o <string> to set the output file name prefix
* -p <string> to override the default control/settings.txt parameters file name

Run modes:
* 0 - read scope traces, decode them and write the data as a ROOT TTree.
* 1 - read scope traces and write them in the binary data format
* 2 - read the binary data format and write out a ROOT TTree.  In this mode the data is read from the input files, rather than from the scope.
