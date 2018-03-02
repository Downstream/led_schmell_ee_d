#pragma once
#pragma once
#ifndef DS_WAFFLES_QUERY_WEATHER_QUERY
#define DS_WAFFLES_QUERY_WEATHER_QUERY

#include <functional>
#include <Poco/Runnable.h>
#include <ds/query/query_result.h>
#include <ds/ui/sprite/sprite_engine.h>
#include "model/generated/media_model.h"
#include "model/weather_data.h"

namespace downstream {

class WeatherQuery : public Poco::Runnable {
public:
	WeatherQuery();

	virtual void									run();

	/// Weather Underground is something like Switzerland/Zurich or OR/Portland. So basically [Country]/[City] or [US State short code]/[City]
	void											setInput(const std::string& wundergroundCity, const std::string& apiKey);

	ds::model::WeatherData							mWeatherData;
	std::string										mWundergroundApiKey;
	std::string										mWundergroundCity;

private:
	void											getWeatherUnderground();
};

}

#endif