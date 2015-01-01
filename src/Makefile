# W. H. Bell
# A Makefile to build scopeDAQ.exe

CC=g++
C=gcc

INCLUDES=Makefile

TARGET=../bin/scopeDAQ.exe

OBJECTS=main.o LeCroyScopeDaq.o LeCroyDataProcessor.o ApplicationBase.o \
LeCroyScope.o BinaryIO.o LeCroyTraceDecoder.o LeCroyDsoTrace.o \
LeCroyDsoBinaryDecoder.o LeCroyDsoTimeStamp.o IOmethods.o lecroy_tcp.o \
RootTreeOutput.o ScopeSettings.o Keyboard.o

INCFLAGS=-I$(shell root-config --incdir) -O
ROOTLIBS = $(shell root-config --libs)

LIBS=$(ROOTLIBS) -ldl 

INCLUDES=Makefile

$(TARGET): $(OBJECTS) $(INCLUDES)
	@echo "**"
	@echo "** Linking Executable"
	@echo "**"
	$(CC) $(OBJECTS) $(LIBS) -o $(TARGET)

clean:
	@rm -f *.o *~

veryclean: clean
	@rm -f $(TARGET)

%.o: %.cxx $(INCLUDES)
	@echo "**"
	@echo "** Compiling C++ Source" 
	@echo "**"
	$(CC) -c $(INCFLAGS) $< 

%.o: %.c $(INCLUDES)
	@echo "**"
	@echo "** Compiling C Source" 
	@echo "**"
	$(C) -c $(INCFLAGS) $<
