#include <iostream>
#include "config/ReadConfig.h"
#include "RawDataInput.h"

#ifndef _HDF5WRITER
#include "writer/HDF5Writer.h"
#endif

#ifndef SPDLOG_VERSION
#include "spdlog/spdlog.h"
#endif

#include <vector>

namespace spd = spdlog;

int main(int argc, char* argv[]){
	auto console = spd::stdout_color_mt("console");
	console->info("RawDataInput started");

	if (argc < 2){
        console->error("Missing argument: <configfile>");
		std::cout << "Usage: rawdatareader <configfile>" << std::endl;
		return 1;
	}

	std::string filename = std::string(argv[1]);
	ReadConfig config = ReadConfig(filename);

	petalo::HDF5Writer writer = petalo::HDF5Writer(&config);
	writer.Open(config.file_out());

	petalo::RawDataInput rdata = petalo::RawDataInput(&config, &writer);
	rdata.readFile(config.file_in());
	rdata.readNext();
	bool hasNext = true;
	while (hasNext){
	     hasNext = rdata.readNext();
	}

	//CLose open files with rawdatainput
	writer.WriteRunInfo();
	std::vector<int> limits = rdata.getLimits();
	printf("limits size: %d\n", limits.size());
	for (int i : limits){
		printf("limits: %d\n", i);
	}
	writer.WriteLimits(rdata.getLimits());
	writer.Close();

	if(!rdata.errors()){
		console->info("RawDataInput finished");
	}else{
		console->info("RawDataInput encountered errors");
	}

	return 0;
}

