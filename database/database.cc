#include <iostream>
#include "database/database.h"

namespace spd = spdlog;

void finish_with_error(MYSQL *con, std::shared_ptr<spdlog::logger> log)
{
  log->error("{}", mysql_error(con));
  mysql_close(con);
  exit(1);
}

void getSensorsFromDB(ReadConfig * config, petalo::Sensors &sensors, int run_number){
	MYSQL *con = mysql_init(NULL);
	std::shared_ptr<spdlog::logger> log = spd::stdout_color_mt("DB");
	std::shared_ptr<spdlog::logger> logerr = spd::stderr_color_mt("mysql");

	if (con == NULL){
		logerr->error("mysql_init() failed");
		exit(1);
	}

	if (mysql_real_connect(con, config->host().c_str(), config->user().c_str(),
				config->pass().c_str(), config->dbname().c_str(), 0, NULL, 0) == NULL){
		finish_with_error(con, logerr);
	}

	//Add run number
	std::string sql = "SELECT CardID, TofpetID, ChannelID, SensorID from ChannelMapping WHERE MinRun <= RUN and MaxRun >= RUN ORDER BY SensorID";

	size_t start_pos = sql.find("RUN");
	while(start_pos != std::string::npos){
		sql.replace(start_pos, 3, std::to_string(run_number));
		start_pos = sql.find("RUN");
	}

	if (mysql_query(con, sql.c_str())){
		finish_with_error(con, logerr);
	}

	MYSQL_RES *result = mysql_store_result(con);
	if (result == NULL){
		finish_with_error(con, logerr);
	}

	log->info("Sensor mapping read from {} in {}", config->dbname(),
			config->host());

	MYSQL_ROW row;

	int card_id, tofpet_id, channel_id, sensor_id;
	while ((row = mysql_fetch_row(result))){
		card_id     = std::stoi(row[0]);
		tofpet_id  = std::stoi(row[1]);
		channel_id = std::stoi(row[2]);
		sensor_id  = std::stoi(row[3]);
		sensors.update_relations(card_id, tofpet_id, channel_id, sensor_id);
		printf("Sensor: %d, %d, %d -> %d\n", card_id, tofpet_id, channel_id, sensor_id);
	}

	mysql_free_result(result);
	mysql_close(con);
}
