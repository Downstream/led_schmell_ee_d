#pragma once
#ifndef _LED_SCHMELL_EE_D_APP_QUERY_QUERYHANDLER_H_
#define _LED_SCHMELL_EE_D_APP_QUERY_QUERYHANDLER_H_

#include <ds/network/helper/delayed_node_watcher.h>
#include <ds/app/event_client.h>
#include <ds/thread/serial_runnable.h>
#include <ds/ui/sprite/sprite_engine.h>
#include "query/story_query.h"
#include "query/weather_query.h"
#include "model/all_data.h"

namespace downstream {

/**
 * \class downstream::QueryHandler
 * \brief Handle app events that deal with querying for data.
 */
class QueryHandler {
public:
	QueryHandler(ds::ui::SpriteEngine&, AllData&);

	void								runQueries();

private:

	void								onAppEvent(const ds::Event&);
	void								runWeatherQuery();

	ds::EventClient						mEventClient;
	ds::ui::SpriteEngine&				mEngine;

	AllData&							mAllData;

	ds::SerialRunnable<StoryQuery>		mStoryQuery;
	ds::SerialRunnable<WeatherQuery>	mWeatherQuery;

	ds::DelayedNodeWatcher				mNodeWatcher;
};

} // !namespace downstream

#endif // !_LED_SCHMELL_EE_D_APP_QUERY_QUERYHANDLER_H_
