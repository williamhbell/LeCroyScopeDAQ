g++ -I../src/ \
../src/LeCroyScopeDaq.o ../src/ApplicationBase.o ../src/LeCroyScope.o ../src/BinaryIO.o \
../src/LeCroyTraceDecoder.o ../src/LeCroyDsoTrace.o ../src/LeCroyDsoBinaryDecoder.o \
../src/LeCroyDsoTimeStamp.o ../src/IOmethods.o ../src/lecroy_tcp.o \
../src/RootTreeOutput.o ../src/ScopeSettings.o \
$(root-config --libs) \
read_waveform.cxx -o read_waveform.exe
