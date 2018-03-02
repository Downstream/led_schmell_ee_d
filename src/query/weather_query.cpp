#include "stdafx.h"

#include "weather_query.h"

#include <algorithm>
#include <map>
#include <sstream>

#include <ds/util/string_util.h>
#include <ds/ui/sprite/sprite_engine.h>
#include <ds/app/engine/engine_cfg.h>
#include <ds/app/environment.h>
#include <ds/debug/logger.h>

#define CURL_STATICLIB
#include "ds/network/curl/curl.h"

namespace downstream {



WeatherQuery::WeatherQuery()
{}

void WeatherQuery::run() {
	mWeatherData.clear();
	getWeatherUnderground();
}

void WeatherQuery::setInput(const std::string& wundergroundCity, const std::string& apiKey) {
	mWundergroundCity = wundergroundCity;
	mWundergroundApiKey = apiKey;
}

size_t weather_write_callback(char *contents, size_t size, size_t nmemb, void *userdata) {
	size_t realsize = size * nmemb;
	((std::string*)userdata)->append((char*)contents, size * nmemb);
	return realsize;
}


void WeatherQuery::getWeatherUnderground() {
	mWeatherData.mError = true;
	mWeatherData.mQueryCity = mWundergroundCity;
	CURL *curl = curl_easy_init();
	if(curl) {

		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30L);

		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		// see https://www.wunderground.com/weather/api/d/docs?d=data/forecast for more info
		std::string inputSS = "http://api.wunderground.com/api/" + mWundergroundApiKey + "/forecast/q/" + mWundergroundCity + ".xml";
		std::string outputStr;

		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, inputSS.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, weather_write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outputStr);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		res = curl_easy_perform(curl);

		//	std::cout << "curl response: " << res << " " << outputStr << std::endl;

		if(res != CURLE_OK) {
			DS_LOG_WARNING("Curl returned a bad response for weather underground! Oh no! " << res);
			return;
		}

		ci::XmlTree xml;

		try {
			xml = ci::XmlTree(outputStr);
		} catch(ci::XmlTree::Exception &e) {
			DS_LOG_WARNING("Weather query couldn't load forecast xml from wunderground city " << mWundergroundCity << ", doc not loaded! oh no: " << e.what());
			//return;
		} catch(std::exception &e) {
			DS_LOG_WARNING("Weather query couldn't load forecast xml from wunderground city " << mWundergroundCity << ", oh no: " << e.what());
			//	return;
		}

		try {

			auto foreDays = xml.getChild("response/forecast/simpleforecast/forecastdays");

			for(auto it = foreDays.begin("forecastday"); it != foreDays.end(); ++it) {
				ds::model::WeatherData::WeatherDayData wdd;
				wdd.mWeatherText = ds::wstr_from_utf8(it->getChild("conditions").getValue());
				wdd.mForecastDate = ds::wstr_from_utf8(it->getChild("date/pretty").getValue());
				wdd.mWeekday = ds::wstr_from_utf8(it->getChild("date/weekday").getValue());
				wdd.mHighTempC = ds::wstr_from_utf8(it->getChild("high/celsius").getValue()) + L"°C";
				wdd.mHighTempF = ds::wstr_from_utf8(it->getChild("high/fahrenheit").getValue()) + L"°F";
				wdd.mLowTempC = ds::wstr_from_utf8(it->getChild("low/celsius").getValue()) + L"°C";
				wdd.mLowTempF = ds::wstr_from_utf8(it->getChild("low/fahrenheit").getValue()) + L"°F";
				//	wdd.mConditionCode = fucking forecast api doesn't give a forecast code. here's a code for you: WTF
				wdd.mIconPath = ds::wstr_from_utf8(it->getChild("icon_url").getValue());
				wdd.mPrecipChance = ds::wstr_from_utf8(it->getChild("pop").getValue()) + L"%";

				mWeatherData.mDayForecasts.push_back(wdd);
			}


			mWeatherData.mError = false;
		} catch(ci::XmlTree::Exception &e) {
			DS_LOG_WARNING("Problems finding child in wunderground city " << mWundergroundCity << " xml reply: " << e.what() << std::endl << std::endl << outputStr);
		} catch(std::exception &e) {
			DS_LOG_WARNING("Problems finding child in wunderground city " << mWundergroundCity << " xml reply: " << e.what());
		}

		inputSS = "http://api.wunderground.com/api/" + mWundergroundApiKey + "/conditions/q/" + mWundergroundCity + ".xml";
		outputStr = "";

		curl_easy_setopt(curl, CURLOPT_URL, inputSS.c_str());
		res = curl_easy_perform(curl);

		if(res != CURLE_OK) {
			DS_LOG_WARNING("Curl returned a bad response for weather underground! Oh no! " << res);
			return;
		}

		ci::XmlTree xmlCond;

		// std::cout << "Current conditions: " << std::endl << outputStr << std::endl;

		try {
			xmlCond = ci::XmlTree(outputStr);
		} catch(ci::XmlTree::Exception &e) {
			DS_LOG_WARNING("Weather query couldn't load conditions xml from wunderground city " << mWundergroundCity << ", doc not loaded! oh no: " << e.what());
		} catch(std::exception &e) {
			DS_LOG_WARNING("Weather query couldn't load conditions xml from wunderground city " << mWundergroundCity << ", oh no: " << e.what());
		}

		try {
			auto foreDays = xmlCond.getChild("response/current_observation");
			for(auto it = foreDays.begin(); it != foreDays.end(); ++it) {
				mWeatherData.mCurrentConditions[it->getTag()] = it->getValue();
			}
		} catch(ci::XmlTree::Exception &e) {
			DS_LOG_WARNING("Problems finding child in wunderground city " << mWundergroundCity << " xml reply: " << e.what() << std::endl << std::endl << outputStr);
		} catch(std::exception &e) {
			DS_LOG_WARNING("Problems finding child in wunderground city " << mWundergroundCity << " xml reply: " << e.what());
		}
	}
}


} // namespace 