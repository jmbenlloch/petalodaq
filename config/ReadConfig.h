#ifndef _READCONFIG
#define _READCONFIG
#endif

#ifndef SPDLOG_VERSION
#include "spdlog/spdlog.h"
#endif

#include <jsoncpp/json/json.h> // or jsoncpp/json.h , or json/json.h etc.

class ReadConfig {
	public:
		ReadConfig(std::string& filename);
		~ReadConfig();

		std::string config();
		std::string file_in();
		std::string file_out();
		int max_events();
		bool two_files();
		void parse();
		int verbosity();
		int extTrigger();
		int skip();
		bool discard();
		bool copyEvts();


	private:
		Json::Reader _reader;
		Json::Value _obj;
		std::string _filename;
		std::string _filein;
		std::string _fileout;
		int _maxevents;
		bool _discard;
		int _verbosity;
		int _skip;
		bool _copyEvts;
		std::shared_ptr<spdlog::logger> _log;
};

inline std::string ReadConfig::config(){return _filename;}

inline std::string ReadConfig::file_in(){ return _filein;}

inline std::string ReadConfig::file_out(){return _fileout;}

inline int ReadConfig::max_events(){return _maxevents;}

inline bool ReadConfig::discard(){return _discard;}

inline int ReadConfig::skip(){return _skip;}

inline int ReadConfig::verbosity(){return _verbosity;}

inline bool ReadConfig::copyEvts(){return _copyEvts;}
