#include "LeCroyScopeDaq.h" 
#include "LeCroyDataProcessor.h"

#include <iostream>
#include <string>
#include <sstream>

void usage(char *);

int main(int argc, char *argv[]) { 
  int iarg;
  std::string strtmp;
  std::istringstream inStr(strtmp);

  // Possible options with defaults
  bool debug = false;
  long numberOfEvents = 1;
  std::string outputFileNamePrefix = "data/scopeData";
  std::string parametersFileName = "control/settings.txt";
  int runMode = 0;
  
  // Now parse options.
  for(iarg=1;iarg<argc;iarg++) {
    strtmp = argv[iarg];

    // Check if this follows the option syntax
    if(strtmp.find("-") == 0) {
      if(strtmp == "-h") {
	usage(argv[0]);
	return 0;
      }
      else if (strtmp == "-d") {
	debug = true;
      }
      else if(strtmp == "-m") {
	iarg++;
	if(iarg>=argc) {
	  std::cerr << "Value missing after " << strtmp << std::endl;
	  usage(argv[0]);
	  return 1;
	}
	strtmp = argv[iarg];
	inStr.clear();
	inStr.str(strtmp);
	inStr >> runMode;
	if(runMode < 0 || runMode > 2) {
	  std::cerr << "Invalid run mode" << std::endl;
	  usage(argv[0]);
	  return 1;
	}
      }
      else if (strtmp == "-n") {
	iarg++;
	if(iarg>=argc) {
	  std::cerr << "Value missing after " << strtmp << std::endl;
	  usage(argv[0]);
	  return 1;
	}
	strtmp = argv[iarg];
	inStr.clear();
	inStr.str(strtmp);
	inStr >> numberOfEvents;
      }
      else if (strtmp == "-o") {
	iarg++;
	if(iarg>=argc) {
	  std::cerr << "Value missing after " << strtmp << std::endl;
	  usage(argv[0]);
	  return 1;
	}
	outputFileNamePrefix = argv[iarg];
      }
      else if (strtmp == "-p") {
	iarg++;
	if(iarg>=argc) {
	  std::cerr << "Value missing after " << strtmp << std::endl;
	  usage(argv[0]);
	  return 1;
	}
	parametersFileName = argv[iarg];
      } 
      else {
	usage(argv[0]);
	return 1;
      }
    }
    else {
      usage(argv[0]);
      return 1;
    }
  }

  if(debug) {
    std::cout << " >> Debug mode on." << std::endl;

    std::cout << " >> Number of events = " << numberOfEvents << std::endl;
    std::cout << " >> Output file prefix = " << outputFileNamePrefix << std::endl;
    if(runMode != 2) {
      std::cout << " >> Parameters file = " << parametersFileName << std::endl;
    }
    std::cout << " >> Run mode = ";
    if (runMode == 0) {
      std::cout << "Scope to Root";
    }
    else if(runMode == 1) {
      std::cout << "Scope to Binary";
    }
    else if(runMode == 2) {
      std::cout << "Binary to Root";
    }
    std::cout << std::endl;
  }

  ApplicationBase *appBase;
  if(runMode == 0) {
    appBase = new LeCroyScopeDaq(numberOfEvents,
				 parametersFileName,
				 outputFileNamePrefix,
				 true, // process data
				 debug);
  }
  else if(runMode == 1) {
    appBase = new LeCroyScopeDaq(numberOfEvents,
				 parametersFileName,
				 outputFileNamePrefix,
				 false, // no processing
				 debug);
  }
  else if(runMode == 2) {
    appBase = new LeCroyDataProcessor(numberOfEvents,
				      outputFileNamePrefix,
				      debug);
  }
  else {
    return 1;
  }

  int retVal;

  // Setup the 'services' and histograms
  if((retVal=appBase->initialize())) {
    return retVal;
  }

  // Loop over the events
  if((retVal=appBase->run())) {
    return retVal;
  }

  // Write any histograms in memory to disk and clean up.
  if((retVal=appBase->finalize())) {
    return retVal;
  }

  delete appBase;

  return 0;
}

//-------------------------------------------------------------

void usage(char *exename) {
  std::cout << " Usage: " << exename << " [options]" << std::endl << std::endl;
  std::cout << " Options:" << std::endl;
  std::cout << "\t -h   Print this message" << std::endl;
  std::cout << "\t -d   Activate debugging mode" << std::endl;
  std::cout << "\t -m <value>     Run mode:" << std::endl;
  std::cout << "\t                0 => scope to root" << std::endl;
  std::cout << "\t                1 => scope to binary" << std::endl;
  std::cout << "\t                2 => binary to root" << std::endl;
  std::cout << "\t -n <value>     Number of events" << std::endl;
  std::cout << "\t -o <value>     Output file prefix or name (when mode == 2)" << std::endl;
  std::cout << "\t -p <value>     Parameter file" << std::endl;
}
