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

	_nodb = config->no_db();
	_ievt = 0;
	_rowData     = 0;
	_rowCounter  = 0;
	_firstEvent = true;
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

	memtype = createEventTimeType();
	table_name = std::string("dateEvents");
	_timesTable = createTable(_file, table_name, memtype);
}

void petalo::HDF5Writer::Close(){
  _isOpen=false;

  _log->debug("Closing output file");
  H5Fclose(_file);
}

void petalo::HDF5Writer::updateSensorID(petalo_t * data){
	int sensor_id = _sensors.elecToSensor(data->card_id, data->tofpet_id, data->channel_id);
	data->sensor_id = sensor_id;
}

void petalo::HDF5Writer::updateSensorID(evt_counter_t * data){
	int sensor_id = _sensors.elecToSensor(data->card_id, data->tofpet_id, data->channel_id);
	data->sensor_id = sensor_id;
}

void petalo::HDF5Writer::Write(std::vector<petalo_t>& tofpetData){
	hsize_t memtype = createTofPetType();

	if (_firstEvent && !_nodb){
		getSensorsFromDB(_config, _sensors, _run_number);
		_firstEvent = false;
	}

	for(int i=0; i<tofpetData.size(); i++){
		_log->debug("Writing event {} to HDF5 file", _rowData);
		petalo_t data = tofpetData.at(i);

		if(!_nodb){
			updateSensorID(&data);
		}

		writeTofPet(&data, _dataTable, memtype, _rowData);
		_rowData++;
	}
	H5Tclose(memtype);
}

void petalo::HDF5Writer::Write(std::vector<evt_counter_t>& tofpetData){
	hsize_t memtype = createEvtCounterType();

	if (_firstEvent && !_nodb){
		getSensorsFromDB(_config, _sensors, _run_number);
		_firstEvent = false;
	}

	for(int i=0; i<tofpetData.size(); i++){
		_log->debug("Writing event {} to HDF5 file", _rowCounter);
		evt_counter_t data = tofpetData.at(i);

		if(!_nodb){
			updateSensorID(&data);
		}

		writeEvtCount(&data, _counterTable, memtype, _rowCounter);
		_rowCounter++;
	}
	H5Tclose(memtype);
}

void petalo::HDF5Writer::WriteLimits(const std::vector<int>& limits){
	if (limits.size() > 0){
		hsize_t memtype = createLimitType();
		std::string run_name = std::string("limits");
		hid_t limits_table = createTable(_file, run_name, memtype);
		limit_t limit_container;
		for (int i=0; i<limits.size(); i++){
			limit_container.limit = limits[i];
			writeLimits(&limit_container, limits_table, memtype, i);
		}
		H5Tclose(memtype);
	}
}

void petalo::HDF5Writer::WriteEventTime(unsigned int evt_number, uint64_t timestamp, unsigned char run_control){
	hsize_t memtype = createEventTimeType();
	evt_time_t evt_time;
	evt_time.evt_number  = evt_number;
	evt_time.run_control = run_control;
	evt_time.timestamp   = timestamp;
	writeEventTime(&evt_time, _timesTable, memtype, _ievt);
	H5Tclose(memtype);
	_ievt += 1;
}


void petalo::HDF5Writer::WriteRunInfo(){
	//Create runInfo table and write run number
	hsize_t memtype = createRunType();
	std::string run_name = std::string("runInfo");
	hid_t runinfo_table = createTable(_file, run_name, memtype);
	runinfo_t runinfo;
	runinfo.run_number = (int) _run_number;
	writeRun(&runinfo, runinfo_table, memtype, 0);
	H5Tclose(memtype);
}
