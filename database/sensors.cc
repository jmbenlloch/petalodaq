#include "database/sensors.h"
#include <iostream>


petalo::Sensors::Sensors(){
}

//Return -1 if not found
int petalo::Sensors::elecToSensor(int card_id, int tofpet_id, int channel_id){
	auto elecID = std::make_tuple(card_id, tofpet_id, channel_id);
	if (_elecToSensor.find(elecID) == _elecToSensor.end()){
		return -1;
	}
	return _elecToSensor[elecID];
}

void petalo::Sensors::update_relations(int card_id, int tofpet_id, int channel_id, int sensorID){
	auto elecID = std::make_tuple(card_id, tofpet_id, channel_id);
	_elecToSensor[elecID]   = sensorID;
}
