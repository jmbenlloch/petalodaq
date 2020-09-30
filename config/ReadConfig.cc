#include <iostream>
#include <fstream>
#include "config/ReadConfig.h"

namespace spd = spdlog;

ReadConfig::ReadConfig(std::string& filename){
	_filename = filename;
	_log = spd::stdout_color_mt("config");
	parse();
}

ReadConfig::~ReadConfig(){
}

void ReadConfig::parse(){
	_log->info("Reading configuration file: {}", _filename);

	std::ifstream ifs(_filename.c_str());
    _reader.parse(ifs, _obj);
	_filein     = _obj["file_in" ].asString();
	_fileout    = _obj["file_out"].asString();
	_maxevents  = _obj.get("max_events", 100000).asInt();
	_verbosity  = _obj.get("verbosity" , 0).asInt();
	_discard    = _obj.get("discard"   , true).asBool();
	_copyEvts   = _obj.get("copy_evts" , false).asBool();
	_skip       = _obj.get("skip"      , 0).asInt();

	_log->info("File in: {}", _filein);
	_log->info("File out: {}", _fileout);
    _log->info("Max events: {}", _maxevents);
	_log->info("Verbosity: {}", _verbosity);
	_log->info("Discard error events: {}", _discard);
	_log->info("Copy events from input: {}", _copyEvts);
	_log->info("Skip events: {}", _skip);
}
