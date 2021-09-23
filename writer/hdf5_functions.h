#ifndef _HDF5FUNCTIONS
#define _HDF5FUNCTIONS
#endif

#include <hdf5.h>
#include <iostream>

#define STRLEN 20

typedef struct{
	int run_number;
} runinfo_t;

typedef struct{
	int limit;
} limit_t;

typedef struct{
	unsigned int evt_number;
	unsigned char run_control;
	uint64_t timestamp;
} evt_time_t;

typedef struct{
	unsigned int evt_number;
	unsigned int ctdaq;
	unsigned char ct_data;
	unsigned short int card_id;
	unsigned char tofpet_id;
	unsigned char wordtype_id;
	unsigned char channel_id;
	short int sensor_id;
	unsigned char tac_id;
	unsigned short int tcoarse;
	unsigned short int ecoarse;
	unsigned short int tfine;
	unsigned short int efine;
} petalo_t;


typedef struct{
	unsigned int  evt_number;
	unsigned short int card_id;
	unsigned char tofpet_id;
	unsigned char wordtype_id;
	unsigned int  reserved;
	unsigned char channel_id;
	short int sensor_id;
	unsigned int  count;
} evt_counter_t;


hid_t createGroup(hid_t file, std::string& group);
hid_t createTable(hid_t group, std::string& table_name, hsize_t memtype);

hid_t createRunType();
hid_t createTofPetType();
hid_t createEvtCounterType();
hid_t createLimitType();
hid_t createEventTimeType();

void writeRun(runinfo_t * runData, hid_t dataset, hid_t memtype, hsize_t evt_number);
void writeLimits(limit_t * limit, hid_t dataset, hid_t memtype, hsize_t row_number);
void writeTofPet(petalo_t * data, hid_t dataset, hid_t memtype, hsize_t evt_number);
void writeEvtCount(evt_counter_t * data, hid_t dataset, hid_t memtype, hsize_t evt_number);
void writeEventTime(evt_time_t * data, hid_t dataset, hid_t memtype, hsize_t evt_number);
