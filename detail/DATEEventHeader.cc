#include "detail/DATEEventHeader.hh"

petalo::DATEEventHeader::DATEEventHeader():
	fEventSize(0),
	fEventMagic(0),
	fEventHeadSize(0),
	fEventVersion(0),
	fEventType(0),
	fRunNb(0),
	fNbInRun(0),
	fBurstNb(0),
	fNbInBurst(0),
	fTimestamp(0)
{
}

petalo::DATEEventHeader::~DATEEventHeader()
{
}
