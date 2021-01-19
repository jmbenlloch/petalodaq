#ifndef _HDF5WRITER
#define _HDF5WRITER
#endif

#include <vector>
#include <iostream>
#include <hdf5.h>

#include "writer/hdf5_functions.h"

#ifndef _READCONFIG
#include "config/ReadConfig.h"
#endif

#ifndef SPDLOG_VERSION
#include "spdlog/spdlog.h"
#endif

namespace petalo{

  class HDF5Writer {

  private:

    //! HDF5 file
    size_t _file;

	bool _isOpen;

	//! First event
	bool _firstEvent;

	//Datasets
	size_t _dataTable;
	size_t _counterTable;

    //! counter for writen events
    size_t _ievt;
    size_t _row;
	size_t _run_number;

	ReadConfig * _config;

	std::shared_ptr<spdlog::logger> _log;

  public:

    //! constructor
    HDF5Writer(ReadConfig * config);

    //! destructor
    virtual ~HDF5Writer();

    //! write event
    void Write(std::vector<petalo_t>&);
    void Write(std::vector<evt_counter_t>&);

    //! open file
    void Open(std::string filename);

    //! close file
    void Close();

    //! write dst info into root file
	hid_t CreateRunInfo(hsize_t file, size_t run_number);
    void WriteRunInfo();

    void SetRunNumber(int run_number);
  };

  inline void HDF5Writer::SetRunNumber(int run_number) {_run_number = run_number;}
}
