#pragma once
#ifndef _LED_SCHMELL_EE_D_APP_MODEL_ALLDATA_H_
#define _LED_SCHMELL_EE_D_APP_MODEL_ALLDATA_H_

#include "model/generated/story_model.h"
#include "model/weather_data.h"

namespace downstream {

/**
 * \class downstream::AllData
 */
class AllData {
public:
	
	AllData(){};

	std::vector<ds::model::StoryRef>	mStories;
	ds::model::WeatherData				mWeatherData;

};

} // !namespace downstream

#endif // !_LED_SCHMELL_EE_D_APP_MODEL_ALLDATA_H_

