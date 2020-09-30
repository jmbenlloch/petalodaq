#include "catch.hpp"

#ifndef SPDLOG_VERSION
#include "spdlog/spdlog.h"
#endif
#include "detail/EventReader.h"


TEST_CASE("Check Bit", "[check_bit]") {
	int mask = 1;

	for(int pos=0; pos<32; pos++){
		for(int i=0; i<32; i++){
			REQUIRE(CheckBit(mask, i) == (int)(i==pos));
		}
		mask = mask << 1;
	}
}

TEST_CASE( "Create event reader", "[eventreader]" ) {
	unsigned short pmt_header[8] = {
		0x0000, //Sequence counter H
		0x0000, //Sequence counter L
		0x0021, //Format ID H
		0xABCD, //Format ID L
		0x0f96, //Wordcount
		0x1234, //Event ID H
		0x5678, //Event ID L
		0x4321, //Card ID
	};

	int16_t * ptr = (int16_t*) pmt_header;
	int16_t * ptr_orig = (int16_t*) pmt_header;

	spdlog::drop("eventreader");
	petalo::EventReader* reader = new petalo::EventReader(0);

    SECTION("Test common header reader" ) {
		reader->ReadCommonHeader(ptr);
		REQUIRE(reader->SequenceCounter() == 0);
    	//Test Format ID
		REQUIRE(reader->GetErrorBit() == 1);
		REQUIRE(reader->FormatType() == 1);
		REQUIRE(reader->FWVersion() == 0xABCD);
    	//Test Word count
		REQUIRE(reader->WordCounter() == 0x0f96);
        //Test Event ID
		REQUIRE(reader->EventID() == 0x12345678);
        //Test Card ID
		REQUIRE(reader->CardID() == 0x4321);

		REQUIRE(ptr == ptr_orig + 8);
	}
}

TEST_CASE( "Test sequence counter", "[seq_counter]" ) {
	unsigned short data[2] = {
		0xFFFF, //Sequence counter H
		0xFFFF, //Sequence counter L
	};

	int16_t * ptr = (int16_t*) data;
	int16_t * ptr_orig = ptr;

	spdlog::drop("eventreader");
	petalo::EventReader* reader = new petalo::EventReader(0);

    SECTION("All active" ) {
		reader->readSeqCounter(ptr);
		REQUIRE(reader->SequenceCounter() == 0xFFFFFFFF);
		REQUIRE(ptr == ptr_orig + 2);
	}

	//spdlogs raise an exception between sections for some reason
	spdlog::drop("eventreader");
	data[0] = 0;
	data[1] = 0;

    SECTION("All inactive" ) {
		reader->readSeqCounter(ptr);
		REQUIRE(reader->SequenceCounter() == 0);
		REQUIRE(ptr == ptr_orig + 2);
	}
}

TEST_CASE( "Test error bit", "[errorbit]" ) {
	unsigned short data[2] = {
		0x0020, // Format ID H
		0x0000, // Format ID L
	};

	int16_t * ptr = (int16_t*) data;
	int16_t * ptr_orig = ptr;

	spdlog::drop("eventreader");
	petalo::EventReader* reader = new petalo::EventReader(0);

    SECTION("All active" ) {
		reader->readFormatID(ptr);
		REQUIRE(reader->GetErrorBit() == 1);
		REQUIRE(ptr == ptr_orig + 2);
	}

	//spdlogs raise an exception between sections for some reason
	spdlog::drop("eventreader");
	data[0] = 0xFFDF;
	data[1] = 0xFFFF;

    SECTION("All inactive" ) {
		reader->readFormatID(ptr);
		REQUIRE(reader->GetErrorBit() == 0);
		REQUIRE(ptr == ptr_orig + 2);
	}
}

TEST_CASE( "Test Format Type", "[ftype]" ) {
	unsigned short data[2] = {
		0x000F, // Format ID H
		0x0000, // Format ID L
	};

	int16_t * ptr = (int16_t*) data;
	int16_t * ptr_orig = ptr;

	spdlog::drop("eventreader");
	petalo::EventReader* reader = new petalo::EventReader(0);

    SECTION("All active" ) {
		reader->readFormatID(ptr);
		REQUIRE(reader->FormatType() == 0x000F);
		REQUIRE(ptr == ptr_orig + 2);
	}

	//spdlogs raise an exception between sections for some reason
	spdlog::drop("eventreader");
	data[0] = 0xFFF0;
	data[1] = 0xFFFF;

    SECTION("All inactive" ) {
		reader->readFormatID(ptr);
		REQUIRE(reader->FormatType() == 0);
		REQUIRE(ptr == ptr_orig + 2);
	}
}

TEST_CASE( "Test FW version", "[fwversion]" ) {
	unsigned short data[2] = {
		0x0000, // Format ID H
		0xFFFF, // Format ID L
	};

	int16_t * ptr = (int16_t*) data;
	int16_t * ptr_orig = ptr;

	spdlog::drop("eventreader");
	petalo::EventReader* reader = new petalo::EventReader(0);

    SECTION("All active" ) {
		reader->readFormatID(ptr);
		REQUIRE(reader->FWVersion() == 0xFFFF);
		REQUIRE(ptr == ptr_orig + 2);
	}

	//spdlogs raise an exception between sections for some reason
	spdlog::drop("eventreader");
	data[0] = 0xFFFF;
	data[1] = 0x0000;

    SECTION("All inactive" ) {
		reader->readFormatID(ptr);
		REQUIRE(reader->FWVersion() == 0);
		REQUIRE(ptr == ptr_orig + 2);
	}
}

TEST_CASE( "Test word count", "[wordcount]" ) {
	unsigned short data[1] = {
		0xFFFF, // wordcount
	};

	int16_t * ptr = (int16_t*) data;
	int16_t * ptr_orig = ptr;

	spdlog::drop("eventreader");
	petalo::EventReader* reader = new petalo::EventReader(0);

    SECTION("All active" ) {
		reader->readWordCount(ptr);
		REQUIRE(reader->WordCounter() == 0xFFFF);
		REQUIRE(ptr == ptr_orig + 1);
	}

	//spdlogs raise an exception between sections for some reason
	spdlog::drop("eventreader");
	data[0] = 0;

    SECTION("All inactive" ) {
		reader->readWordCount(ptr);
		REQUIRE(reader->WordCounter() == 0);
		REQUIRE(ptr == ptr_orig + 1);
	}
}

TEST_CASE( "Test Event ID", "[evtid]" ) {
	unsigned short data[2] = {
		0xFFFF, // Event ID H
		0xFFFF, // Event ID L
	};

	int16_t * ptr = (int16_t*) data;
	int16_t * ptr_orig = ptr;

	spdlog::drop("eventreader");
	petalo::EventReader* reader = new petalo::EventReader(0);

    SECTION("All active" ) {
		reader->readEventID(ptr);
		REQUIRE(reader->EventID() == 0xFFFFFFFF);
		REQUIRE(ptr == ptr_orig + 2);
	}

	//spdlogs raise an exception between sections for some reason
	spdlog::drop("eventreader");
	data[0] = 0x0000;
	data[1] = 0x0000;

    SECTION("All inactive" ) {
		reader->readEventID(ptr);
		REQUIRE(reader->EventID() == 0);
		REQUIRE(ptr == ptr_orig + 2);
	}
}

TEST_CASE( "Test Card ID", "[cardid]" ) {
	unsigned short data[1] = {
		0xFFFF, // cardid
	};

	int16_t * ptr = (int16_t*) data;
	int16_t * ptr_orig = ptr;

	spdlog::drop("eventreader");
	petalo::EventReader* reader = new petalo::EventReader(0);

    SECTION("All active" ) {
		reader->readCardID(ptr);
		REQUIRE(reader->CardID() == 0xFFFF);
		REQUIRE(ptr == ptr_orig + 1);
	}

	//spdlogs raise an exception between sections for some reason
	spdlog::drop("eventreader");
	data[0] = 0;

    SECTION("All inactive" ) {
		reader->readCardID(ptr);
		REQUIRE(reader->CardID() == 0);
		REQUIRE(ptr == ptr_orig + 1);
	}
}
