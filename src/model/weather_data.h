#pragma once
#ifndef DS_MODEL_WEATHER_DATA_MODEL_H_
#define DS_MODEL_WEATHER_DATA_MODEL_H_

#include <string>
#include <vector>
#include <map>

namespace ds {
namespace model {

class WeatherData {
public:
	struct WeatherCodes {
		/*
		1	Clear
		2	Partly Cloudy
		3	Mostly Cloudy
		4	Cloudy
		5	Hazy
		6	Foggy
		7	Very Hot
		8	Very Cold
		9	Blowing Snow
		10	Chance of Showers
		11	Showers
		12	Chance of Rain
		13	Rain
		14	Chance of a Thunderstorm
		15	Thunderstorm
		16	Flurries
		17	OMITTED
		18	Chance of Snow Showers
		19	Snow Showers
		20	Chance of Snow
		21	Snow
		22	Chace of Ice Pellets
		23	Ice Pellets
		24	Blizzard
		*/
	};


	class WeatherDayData {
	public:
		WeatherDayData() :mConditionCode(-1) {}

		std::wstring			mWeatherText; // Rain, Sunny, Tornado, etc.
		std::wstring			mForecastDate; // 7:00 PM PDT on October 12, 2017
		std::wstring			mWeekday; // Thursday
		std::wstring			mHighTempC; // 35°C
		std::wstring			mHighTempF; // 70°F
		std::wstring			mLowTempC; // 11°C
		std::wstring			mLowTempF; // 30°F
		std::wstring			mIconPath; // http://icons.wxug.com/i/c/k/chancerain.gif
		std::wstring			mPrecipChance; // 30%

											   // likely not supported by forecast api...
		int						mConditionCode; // weather condition code https://www.wunderground.com/weather/api/d/docs?d=resources/phrase-glossary
	};

	WeatherData() :mError(false) {}

	void						clear() {
		mError = true;
		mCity = "";
		mDayForecasts.clear();
	}

	// If the query to get the weather failed
	bool						mError;

	/// A key / value pair of conditions (for example: temp_f, 66.3)
	std::map<std::string, std::string>	mCurrentConditions;

	// Printable, human-readable City text (e.g. Portland, OR)
	std::string					mCity;
	// The city to put into the api (e.g. OR/Portland)
	std::string					mQueryCity;

	// As many days as possible of forecasts and conditions
	// starts at today, and advances each day.
	// Today is mDayForecasts[0], tomorrow is mDayForecasts[1], etc.
	// Check size before using, will be empty if error is true
	std::vector<WeatherDayData>	mDayForecasts;
};

}
}
#endif