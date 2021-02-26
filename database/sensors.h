#ifndef DATABASE_H
#define DATABASE_H
#include <map>
#include <vector>
#include <tuple>

#define NSIPM 1792
#define NPMT 12

typedef std::tuple<int, int, int> sensorTuple;

namespace petalo{
	class Sensors
	{
		public:
			Sensors();

			int elecToSensor(int card_id, int tofpet_id, int channel_id);

			void update_relations(int card_id, int tofpet_id, int channel_id, int sensorID);

		private:
			std::map<sensorTuple, int> _elecToSensor;
	};

}

#endif
