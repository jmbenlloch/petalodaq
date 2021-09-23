#ifndef _EVENTREADER
#define _EVENTREADER
#endif

#ifndef SPDLOG_VERSION
#include "spdlog/spdlog.h"
#endif

#include <vector>
#include <iostream>
#include <stdint.h>
#include <string.h>

namespace petalo {

  class EventReader
  {
  public:
    /// Default constructor
    EventReader(int verbose);
    /// Destructor
    ~EventReader();

    void FlipWords(int16_t* &, int16_t* &);
    void ReadCommonHeader(int16_t* &ptr);
	void readSeqCounter(int16_t* &ptr);
	void readFormatID(int16_t* &ptr);
	void readWordCount(int16_t* &ptr);
	void readEventID(int16_t* &ptr);
	void readCardID(int16_t* &ptr);
	void readCTDaq(int16_t* &ptr);

    unsigned int FWVersion() const;
    unsigned int FormatType() const;
    unsigned int RunMode() const;
    unsigned char RunControl() const;
    unsigned int EmptyBit() const;
    unsigned int SequenceCounter() const;
    unsigned int WordCounter() const;
    unsigned int EventID() const;
    unsigned int CardID() const;
    int GetErrorBit() const;
	unsigned int GetCTDaq() const;

  private:
    unsigned int fFWVersion;
    unsigned int fFormatType;
    unsigned int fEmpty;
    unsigned int fRunMode;
    unsigned char fRunControl;
    unsigned int fSequenceCounter;
    unsigned int fWordCounter;
	unsigned int fCardID;
    int fChannelMask;
    int fErrorBit;
	int fEventID;
	int verbose_;
	unsigned int ctdaq;

	std::shared_ptr<spdlog::logger> _log;

  }; //class EventReader

  typedef std::vector<EventReader> EventReaderCollection;

  // INLINE METHODS //////////////////////////////////////////////////

  inline unsigned int EventReader::FWVersion() const {return fFWVersion;}
  inline unsigned int EventReader::EmptyBit() const {return fEmpty;}
  inline unsigned int EventReader::RunMode() const {return fRunMode;}
  inline unsigned int EventReader::FormatType() const {return fFormatType;}
  inline unsigned int EventReader::SequenceCounter() const {return fSequenceCounter;}
  inline unsigned int EventReader::WordCounter() const {return fWordCounter;}
  inline unsigned int EventReader::CardID() const {return fCardID;}
  inline unsigned int EventReader::EventID() const {return fEventID;}
  inline unsigned char EventReader::RunControl() const {return fRunControl;}
  inline int EventReader::GetErrorBit() const {return fErrorBit;}
  inline unsigned int EventReader::GetCTDaq() const {return ctdaq;}


}

int CheckBit(int, int);
