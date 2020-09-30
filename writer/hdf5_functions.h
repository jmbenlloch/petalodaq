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
	unsigned int evt_number;
	unsigned char tofpet_id;
	unsigned char wordtype_id;
	unsigned char channel_id;
	unsigned char tac_id;
	unsigned short int tcoarse;
	unsigned short int ecoarse;
	unsigned short int tfine;
	unsigned short int efine;
} petalo_t;


hid_t createGroup(hid_t file, std::string& group);
hid_t createTable(hid_t group, std::string& table_name, hsize_t memtype);

hid_t createRunType();
hid_t createTofPetType();

void writeRun(runinfo_t * runData, hid_t dataset, hid_t memtype, hsize_t evt_number);
void writeTofPet(petalo_t * data, hid_t dataset, hid_t memtype, hsize_t evt_number);
