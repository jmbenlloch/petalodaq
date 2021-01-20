#include "writer/HDF5Writer.h"
#include <sstream>
#include <cstring>
#include <stdlib.h>

#include<stdint.h>

namespace spd = spdlog;

petalo::HDF5Writer::HDF5Writer(ReadConfig * config) :
    _config(config)
{

	_log = spd::stdout_color_mt("writer");
	if(config->verbosity() > 0){
		_log->set_level(spd::level::debug);
	}

	_ievt = 0;
	_row  = 0;
}

petalo::HDF5Writer::~HDF5Writer(){
}

void petalo::HDF5Writer::Open(std::string fileName){
	_log->debug("Opening output file {}", fileName);

	_file = H5Fcreate( fileName.c_str(), H5F_ACC_TRUNC,
			H5P_DEFAULT, H5P_DEFAULT );

	_isOpen=true;


	//Create triggerType table
	hsize_t memtype = createTofPetType();
	std::string table_name = std::string("data");
	_dataTable = createTable(_file, table_name, memtype);

	memtype = createEvtCounterType();
	table_name = std::string("counter");
	_counterTable = createTable(_file, table_name, memtype);
}

void petalo::HDF5Writer::Close(){
  _isOpen=false;

  _log->debug("Closing output file");
  H5Fclose(_file);
}

void petalo::HDF5Writer::Write(std::vector<petalo_t>& tofpetData){
	hsize_t memtype = createTofPetType();

	printf("writer: tofpet size: %d\n", tofpetData.size());
	printf("row: %d\n", _row);

	for(int i=0; i<tofpetData.size(); i++){
		_log->debug("Writing event {} to HDF5 file", _row);
		writeTofPet( &(tofpetData.at(i)), _dataTable, memtype, _row);
		_row++;
	}
}

void petalo::HDF5Writer::Write(std::vector<evt_counter_t>& tofpetData){
	hsize_t memtype = createEvtCounterType();

	printf("writer: evt counter size: %d\n", tofpetData.size());
	printf("row: %d\n", _row);

	for(int i=0; i<tofpetData.size(); i++){
		_log->debug("Writing event {} to HDF5 file", _row);
		writeEvtCount( &(tofpetData.at(i)), _counterTable, memtype, _row);
		_row++;
	}
}

void petalo::HDF5Writer::WriteRunInfo(){
	//Create runInfo table and write run number
	hsize_t memtype = createRunType();
	std::string run_name = std::string("runInfo");
	hid_t runinfo_table = createTable(_file, run_name, memtype);
	runinfo_t runinfo;
	runinfo.run_number = (int) _run_number;
	writeRun(&runinfo, runinfo_table, memtype, 0);
}
