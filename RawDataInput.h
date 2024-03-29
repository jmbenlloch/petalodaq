////////////////////////////////////////////////////////////////////////
// RawDataInput
//
// Reads the files provided by the DAQ
//
////////////////////////////////////////////////////////////////////////

#ifndef _READCONFIG
#include "config/ReadConfig.h"
#endif

#ifndef SPDLOG_VERSION
#include "spdlog/spdlog.h"
#endif

#include "detail/DATEEventHeader.hh"

#ifndef _HDF5WRITER
#include "writer/HDF5Writer.h"
#endif

#ifndef _HDF5EVENTREADER
#include "detail/EventReader.h"
#endif

#include "detail/event.h"

#include <stdint.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <numeric>
#include <ios>
#include <unistd.h>

#define MEMSIZE 8500000

namespace petalo {

class RawDataInput {

public:
  RawDataInput();
  RawDataInput(ReadConfig * config, HDF5Writer * writer);

  /// Open specified file.
  void readFile(std::string const & filename);
  std::FILE* openDATEFile(std::string const & filename);
  void countEvents(std::FILE* file, int * events, int * firstEvt);
  int loadNextEvent(std::FILE* file, unsigned char ** buffer);

  /// Read an event.
  bool readNext();

  ///Function to read DATE information
  bool ReadDATEEvent();

  ///Fill PMT classes
  void writeEvent();

  bool errors();

  int decodeTofPet(int16_t * buffer, std::vector<petalo_t>& data, unsigned int evt_number, int cardID, unsigned int ctdaq);
  int decodeEventCounter(int16_t * buffer, std::vector<evt_counter_t>& data, unsigned int evt_number, int cardID);
  void ReadTofPet(int16_t * buffer, unsigned int size, int RunMode);

  std::vector<int> getLimits();

private:
  /// Retrieve DATE event header size stored in the raw data.
  /// 80 bytes for the newer DAQ (DATE event header format 3.14)
  unsigned int readHeaderSize(std::FILE* fptr) const;

  std::unique_ptr<petalo::EventHeaderCollection> headOut_;

  size_t run_;
  std::FILE* fptr_; // current fptr
  int entriesThisFile_;
  eventHeaderStruct * event_;      // raw data super event
  int eventNo_;
  int skip_;
  int max_events_;
  unsigned char* buffer_;

  // verbosity control
  unsigned int verbosity_;///< default 0 for quiet output.

  ///New EventReader class
  petalo::EventReader *eventReader_;

  //If true, discard FECs with ErrorBit true
  bool discard_;

  std::shared_ptr<spdlog::logger> _log;
  std::shared_ptr<spdlog::logger> _logerr;

  petalo::HDF5Writer * _writer;

  int fwVersion;
  int fwVersionPmt;
  int previousType_;
  int positionInCountTable_;

  bool fileError_, eventError_;
  ReadConfig * config_;

  std::unique_ptr<std::vector<petalo_t> > dataVector_;
  std::unique_ptr<std::vector<evt_counter_t> > countVector_;
  std::unique_ptr<std::vector<int> > limitsVector_;

};

inline bool RawDataInput::errors(){return fileError_;}
inline std::vector<int> RawDataInput::getLimits(){return *limitsVector_;};

}

int flipWords(unsigned int size, int16_t* in, int16_t* out);
bool isEventSelected(eventHeaderStruct& event);
