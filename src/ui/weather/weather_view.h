#pragma once
#ifndef _LED_SCHMELL_EE_D_APP_UI_WEATHER_WEATHER_VIEW_H_
#define _LED_SCHMELL_EE_D_APP_UI_WEATHER_WEATHER_VIEW_H_


#include <ds/ui/sprite/sprite.h>
#include <ds/ui/layout/smart_layout.h>

#include "model/generated/story_model.h"

namespace downstream {

class Globals;

class WeatherView : public ds::ui::SmartLayout {
public:
	WeatherView(Globals& g);

private:

	void								animateOn();
	void								animateOff();

	void								setData();

	Globals&							mGlobals;


};

} // namespace downstream

#endif

#pragma once
