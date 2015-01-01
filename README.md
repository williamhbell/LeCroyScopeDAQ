LeCroyScopeDAQ
==============

Command line DAQ for LeCroy X-Stream oscilloscopes
Author: W. H. Bell

Purpose
-------
General purpose program for acquiring scope traces from a LeCroy
X-Stream oscilloscope.

Description
-----------
The scope parameters are set from the values given in the
control/settings.txt file.  The scope is then setup and readout for a
given number of triggers.  The data taken from the scope are decoded and recorded in a Root TTree saved in a root file.

------------------------------------------------------

(1) Build the code
------------------
cd src
make
cd ..

(2) Setup the Scope Parameters
------------------------------

control/settings.txt
	The parameter name is given followed by at least one space or
	tab, and then the value for the parameter.

(3) Run the code
----------------
./bin/scopeDAQ.exe
