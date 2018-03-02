#ifndef _LED_SCHMELL_EE_D_APP_APPEVENTS_H_
#define _LED_SCHMELL_EE_D_APP_APPEVENTS_H_

#include <ds/app/event.h>

namespace downstream {

class StoryDataUpdatedEvent : public ds::RegisteredEvent<StoryDataUpdatedEvent>{
public:
	StoryDataUpdatedEvent(){};
};

class WeatherDataUpdatedEvent : public ds::RegisteredEvent<WeatherDataUpdatedEvent> {
public:
	WeatherDataUpdatedEvent() {};
};

class RequestAppExitEvent : public ds::RegisteredEvent<RequestAppExitEvent>{
public:
	RequestAppExitEvent(){};
};

} // !namespace downstream

#endif // !_LED_SCHMELL_EE_D_APP_APPEVENTS_H_
