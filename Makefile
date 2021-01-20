CC = g++
CXXFLAGS = -g -ljsoncpp -O3 -std=c++11 -Wall -Wextra -pedantic -pthread -lhdf5
INCFLAGS = -I. -I$(HDF5INC)

CXXFLAGS += '-DHDF5'

all: config eventreader writer decode link 

tests: 
	$(CC) -o tests ReadConfig.o RawDataInput.o DATEEventHeader.o EventReader.o HDF5Writer.o hdf5_functions.o testing/*cc $(CXXFLAGS) $(INCFLAGS)

link:
	$(CC) -g -o decode decode.o ReadConfig.o RawDataInput.o DATEEventHeader.o EventReader.o HDF5Writer.o hdf5_functions.o $(CXXFLAGS) $(INCFLAGS) -I$(JSONINC) -L$(HDF5LIB)

decode:
	$(CC) -c decode.cc $(CXXFLAGS) $(INCFLAGS)

config:
	$(CC) -c config/ReadConfig.cc $(CXXFLAGS) $(INCFLAGS)

eventreader:
	$(CC) -c detail/*.cc $(CXXFLAGS) $(INCFLAGS)
	$(CC) -c RawDataInput.cc $(CXXFLAGS) $(INCFLAGS)

writer:
	$(CC) -c writer/*cc $(CXXFLAGS) $(INCFLAGS)

clean:
	@rm *.o decode

.PHONY: decode config clean eventreader writer tests petaloreader petalo
