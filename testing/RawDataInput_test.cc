#include "catch.hpp"
#include "RawDataInput.h"
#ifndef _HUFFMAN
#endif

TEST_CASE("Test flip words", "[flip_words]") {
	const unsigned int size = 16;
	unsigned short data[size] = {0x0123, 0x4567, 0x89ab, 0xcdef,
		0xfedc,	0xba98, 0x7654, 0x3210, 0xffff, 0xffff, 0xfafa, 0xfafa};

	unsigned short flip[size] = {0x4567, 0x0123, 0xcdef, 0x89ab,
		0xba98, 0xfedc, 0x3210, 0x7654, 0xffff, 0xffff, 0xfafa, 0xfafa};

	int16_t * flip_ptr = (int16_t*) flip;
	int16_t test_data[size];

	flipWords(size, (int16_t*)data, test_data);

	unsigned int size_check = size / 2; // FAFAFAFA won't be copied

	for(unsigned int i=0; i < size_check; i++){
		REQUIRE(test_data[i] == flip_ptr[i]);
	}
}

TEST_CASE("Test seq counter removal", "[seqcounter_removal]") {
	const unsigned int size = 40000;
	int dataframe_size = 3996;
	int16_t data[size];
	int16_t test_data[size];
	memset(data, 0, sizeof(int16_t) * size);
	memset(test_data, 1, sizeof(int16_t) * size);

	//Write sequence counters in the data
	for(unsigned int i=0; i < size/dataframe_size; i++){
		data[dataframe_size*i + 1] = i;
	}

	flipWords(size, data, test_data);

	//Sequence counters will be removed, so the actual data
	//copied will be smaller than size
	unsigned int size_check = size/2 - size/dataframe_size * 2;

	for(unsigned int i=0; i<size_check; i++){
		REQUIRE(test_data[i] == 0);
	}
}

TEST_CASE("Selected event", "[selected_event]") {
	eventHeaderStruct event;

	//Event types currently goes from 1 to 14
	for(unsigned int i=0; i<16; i++){
		event.eventType = i;
		bool condition = (i == PHYSICS_EVENT) || (i == CALIBRATION_EVENT);
		REQUIRE(isEventSelected(event) == condition);
	}
}

TEST_CASE("Decode TOF PET", "[decode_tofpet]") {
	unsigned short data[5] = {
		0xE08A, // tofpet_id = 7, word_type=0b10, channel_id = 0x0A
		0xBF0F, // tac_id = 2, tcoarse =  0xFC3F
		0xC00F, // ecoarse = 0, tfine = 0x3FF
		0xFD5C, // efine = 0x15C
	};
	unsigned int evt_number = 345;

	int16_t * ptr = (int16_t*) data;

	std::vector<petalo_t> dataVector;

	petalo::RawDataInput rdata = petalo::RawDataInput();
	int offset = rdata.decodeTofPet(ptr, dataVector, evt_number);


	REQUIRE(dataVector[0].evt_number  == evt_number);
	REQUIRE(dataVector[0].tofpet_id   == 7);
	REQUIRE(dataVector[0].wordtype_id == 2);
	REQUIRE(dataVector[0].channel_id  == 0x0A);
	REQUIRE(dataVector[0].tac_id      == 2);
	REQUIRE(dataVector[0].tcoarse     == 0xFC3F);
	REQUIRE(dataVector[0].ecoarse     == 0);
	REQUIRE(dataVector[0].tfine       == 0x3FF);
	REQUIRE(dataVector[0].efine       == 0x15C);

	REQUIRE(offset == 4);
}


TEST_CASE("Decode tofpet ID", "[decode_tofpet_id]") {
	unsigned short data[5] = {
		0xE000,
		0x0000,
		0x0000,
		0x0000,
	};
	unsigned int evt_number = 0;

	int16_t * ptr = (int16_t*) data;

	std::vector<petalo_t> dataVector;

	petalo::RawDataInput rdata = petalo::RawDataInput();
	int offset = rdata.decodeTofPet(ptr, dataVector, evt_number);

	SECTION("All active"){
		REQUIRE(dataVector[0].tofpet_id == 7);
		REQUIRE(offset == 4);
	}

	dataVector.clear();
	data[0] = 0x1FFF;
	data[1] = 0xFFFF;
	data[2] = 0xFFFF;
	data[3] = 0xFFFF;

	offset = rdata.decodeTofPet(ptr, dataVector, evt_number);
	SECTION("All inactive"){
		REQUIRE(dataVector[0].tofpet_id == 0);
		REQUIRE(offset == 4);
	}
}

TEST_CASE("Decode word type", "[decode_word_type]") {
	unsigned short data[5] = {
		0x00C0,
		0x0000,
		0x0000,
		0x0000,
	};
	unsigned int evt_number = 0;

	int16_t * ptr = (int16_t*) data;

	std::vector<petalo_t> dataVector;

	petalo::RawDataInput rdata = petalo::RawDataInput();
	int offset = rdata.decodeTofPet(ptr, dataVector, evt_number);

	SECTION("All active"){
		REQUIRE(dataVector[0].wordtype_id == 0x3);
		REQUIRE(offset == 4);
	}

	dataVector.clear();
	data[0] = 0xFF3F;
	data[1] = 0xFFFF;
	data[2] = 0xFFFF;
	data[3] = 0xFFFF;

	offset = rdata.decodeTofPet(ptr, dataVector, evt_number);
	SECTION("All inactive"){
		REQUIRE(dataVector[0].wordtype_id == 0);
		REQUIRE(offset == 4);
	}
}


TEST_CASE("Decode channel ID", "[decode_channel_id]") {
	unsigned short data[5] = {
		0x003F,
		0x0000,
		0x0000,
		0x0000,
	};
	unsigned int evt_number = 0;

	int16_t * ptr = (int16_t*) data;

	std::vector<petalo_t> dataVector;

	petalo::RawDataInput rdata = petalo::RawDataInput();
	int offset = rdata.decodeTofPet(ptr, dataVector, evt_number);

	SECTION("All active"){
		REQUIRE(dataVector[0].channel_id == 0x3F);
		REQUIRE(offset == 4);
	}

	dataVector.clear();
	data[0] = 0xFFC0;
	data[1] = 0xFFFF;
	data[2] = 0xFFFF;
	data[3] = 0xFFFF;

	offset = rdata.decodeTofPet(ptr, dataVector, evt_number);
	SECTION("All inactive"){
		REQUIRE(dataVector[0].channel_id == 0);
		REQUIRE(offset == 4);
	}
}

TEST_CASE("Decode TAC ID", "[decode_tac_id]") {
	unsigned short data[5] = {
		0x0000,
		0xC000,
		0x0000,
		0x0000,
	};
	unsigned int evt_number = 0;

	int16_t * ptr = (int16_t*) data;

	std::vector<petalo_t> dataVector;

	petalo::RawDataInput rdata = petalo::RawDataInput();
	int offset = rdata.decodeTofPet(ptr, dataVector, evt_number);

	SECTION("All active"){
		REQUIRE(dataVector[0].tac_id == 3);
		REQUIRE(offset == 4);
	}

	dataVector.clear();
	data[0] = 0xFFFF;
	data[1] = 0x3FFF;
	data[2] = 0xFFFF;
	data[3] = 0xFFFF;

	offset = rdata.decodeTofPet(ptr, dataVector, evt_number);
	SECTION("All inactive"){
		REQUIRE(dataVector[0].tac_id == 0);
		REQUIRE(offset == 4);
	}
}



TEST_CASE("Decode tcoarse", "[decode_tcoarse]") {
	unsigned short data[5] = {
		0x0000,
		0x3FFF,
		0xC000,
		0x0000,
	};
	unsigned int evt_number = 0;

	int16_t * ptr = (int16_t*) data;

	std::vector<petalo_t> dataVector;

	petalo::RawDataInput rdata = petalo::RawDataInput();
	int offset = rdata.decodeTofPet(ptr, dataVector, evt_number);

	SECTION("All active"){
		REQUIRE(dataVector[0].tcoarse == 0x0FFFF);
		REQUIRE(offset == 4);
	}

	dataVector.clear();
	data[0] = 0xFFFF;
	data[1] = 0xC000;
	data[2] = 0x3FFF;
	data[3] = 0xFFFF;

	offset = rdata.decodeTofPet(ptr, dataVector, evt_number);
	SECTION("All inactive"){
		REQUIRE(dataVector[0].tcoarse == 0);
		REQUIRE(offset == 4);
	}
}


TEST_CASE("Decode ecoarse", "[decode_ecoarse]") {
	unsigned short data[5] = {
		0x0000,
		0x0000,
		0x3FF0,
		0x0000,
	};
	unsigned int evt_number = 0;

	int16_t * ptr = (int16_t*) data;

	std::vector<petalo_t> dataVector;

	petalo::RawDataInput rdata = petalo::RawDataInput();
	int offset = rdata.decodeTofPet(ptr, dataVector, evt_number);

	SECTION("All active"){
		REQUIRE(dataVector[0].ecoarse == 0x3FF);
		REQUIRE(offset == 4);
	}

	dataVector.clear();
	data[0] = 0xFFFF;
	data[1] = 0xFFFF;
	data[2] = 0xC00F;
	data[3] = 0xFFFF;

	offset = rdata.decodeTofPet(ptr, dataVector, evt_number);
	SECTION("All inactive"){
		REQUIRE(dataVector[0].ecoarse == 0);
		REQUIRE(offset == 4);
	}
}


TEST_CASE("Decode tfine", "[decode_tfine]") {
	unsigned short data[5] = {
		0x0000,
		0x0000,
		0x000F,
		0xFC00,
	};
	unsigned int evt_number = 0;

	int16_t * ptr = (int16_t*) data;

	std::vector<petalo_t> dataVector;

	petalo::RawDataInput rdata = petalo::RawDataInput();
	int offset = rdata.decodeTofPet(ptr, dataVector, evt_number);

	SECTION("All active"){
		REQUIRE(dataVector[0].tfine == 0x3FF);
		REQUIRE(offset == 4);
	}

	dataVector.clear();
	data[0] = 0xFFFF;
	data[1] = 0xFFFF;
	data[2] = 0xFFF0;
	data[3] = 0x03FF;

	offset = rdata.decodeTofPet(ptr, dataVector, evt_number);
	SECTION("All inactive"){
		REQUIRE(dataVector[0].tfine == 0);
		REQUIRE(offset == 4);
	}
}


TEST_CASE("Decode efine", "[decode_efine]") {
	unsigned short data[5] = {
		0x0000,
		0x0000,
		0x0000,
		0x03FF,
	};
	unsigned int evt_number = 0;

	int16_t * ptr = (int16_t*) data;

	std::vector<petalo_t> dataVector;

	petalo::RawDataInput rdata = petalo::RawDataInput();
	int offset = rdata.decodeTofPet(ptr, dataVector, evt_number);

	SECTION("All active"){
		REQUIRE(dataVector[0].efine == 0x3FF);
		REQUIRE(offset == 4);
	}

	dataVector.clear();
	data[0] = 0xFFFF;
	data[1] = 0xFFFF;
	data[2] = 0xFFFF;
	data[3] = 0xFC00;

	offset = rdata.decodeTofPet(ptr, dataVector, evt_number);
	SECTION("All inactive"){
		REQUIRE(dataVector[0].efine == 0);
		REQUIRE(offset == 4);
	}
}

