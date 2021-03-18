#ifndef navel_Products_DATEEventHeader_hh
#define navel_Products_DATEEventHeader_hh
#include <vector>
#include <stdint.h>

namespace petalo {

  class DATEEventHeader
  {
  public:
    /// Default constructor
    DATEEventHeader();
    /// Destructor
    ~DATEEventHeader();

    unsigned int EventSize() const;
    void SetEventSize(unsigned int);

    unsigned int EventMagic() const;
    void SetEventMagic(unsigned int);

    unsigned int EventHeadSize() const;
    void SetEventHeadSize(unsigned int);

    unsigned int EventVersion() const;
    void SetEventVersion(unsigned int);

    unsigned int EventType() const;
    void SetEventType(unsigned int);

    unsigned int RunNb() const;
    void SetRunNb(unsigned int);

    unsigned int NbInRun() const;
    void SetNbInRun(unsigned int nb);

    unsigned int BurstNb() const;
    void SetBurstNb(unsigned int);

    unsigned int NbInBurst() const;
    void SetNbInBurst(unsigned int);

    uint64_t Timestamp() const;
    void SetTimestamp(uint64_t);

  private:
    unsigned int fEventSize;     ///< event size
    unsigned int fEventMagic;    ///< magic event signature
    unsigned int fEventHeadSize; ///< event header size
    unsigned int fEventVersion;  ///< event version
    unsigned int fEventType;     ///< event type
    unsigned int fRunNb;         ///< event run number
    unsigned int fNbInRun;       ///< event number within run
    unsigned int fBurstNb;       ///< burst number
    unsigned int fNbInBurst;     ///< event number within burst
	uint64_t     fTimestamp;     ///< DATE event timestamp in us

  }; //class DATEEventHeader

  typedef std::vector<DATEEventHeader> EventHeaderCollection;

  // INLINE METHODS //////////////////////////////////////////////////

  inline unsigned int DATEEventHeader::EventSize() const
  { return fEventSize; }
  inline void DATEEventHeader::SetEventSize(unsigned int size)
  { fEventSize = size; }

  inline unsigned int DATEEventHeader::EventMagic() const
  { return fEventMagic; }
  inline void DATEEventHeader::SetEventMagic(unsigned int magic)
  { fEventMagic = magic; }

  inline unsigned int DATEEventHeader::EventHeadSize() const
  { return fEventHeadSize; }
  inline void DATEEventHeader::SetEventHeadSize(unsigned int size)
  { fEventHeadSize = size; }

  inline unsigned int DATEEventHeader::EventVersion() const
  { return fEventVersion; }
  inline void DATEEventHeader::SetEventVersion(unsigned int version)
  { fEventVersion = version; }

  inline unsigned int DATEEventHeader::EventType() const
  { return fEventType; }
  inline void DATEEventHeader::SetEventType(unsigned int type)
  { fEventType = type; }

  inline unsigned int DATEEventHeader::RunNb() const
  { return fRunNb; }
  inline void DATEEventHeader::SetRunNb(unsigned int nb)
  { fRunNb = nb; }

  inline unsigned int DATEEventHeader::NbInRun() const
  { return fNbInRun; }
  inline void DATEEventHeader::SetNbInRun(unsigned int nb)
  { fNbInRun = nb; }

  inline unsigned int DATEEventHeader::BurstNb() const
  { return fBurstNb; }
  inline void DATEEventHeader::SetBurstNb(unsigned int nb)
  { fBurstNb = nb; }

  inline unsigned int DATEEventHeader::NbInBurst() const
  { return fNbInBurst; }
  inline void DATEEventHeader::SetNbInBurst(unsigned int nb)
  { fNbInBurst = nb; }

  inline uint64_t DATEEventHeader::Timestamp() const
  { return fTimestamp; }
  inline void DATEEventHeader::SetTimestamp(uint64_t timestamp)
  { fTimestamp = timestamp; }

} // namespace petalo

#endif /*  */
