#include "stdafx.h"

#include "query_handler.h"

#include <ds/app/event_notifier.h>
#include "app/globals.h"
#include "events/app_events.h"

namespace downstream {

QueryHandler::QueryHandler(ds::ui::SpriteEngine& se, AllData &ad)
	: mEngine(se)
	, mEventClient(se.getNotifier(), [this](const ds::Event* e) {if(e) onAppEvent(*e); })
	, mAllData(ad)
	, mNodeWatcher(se)
	, mStoryQuery(se, []() {return new StoryQuery(); })
	, mWeatherQuery(se, [] { return new WeatherQuery(); })
{

	// Initialize data
	mStoryQuery.setReplyHandler([this](StoryQuery& q){
		mAllData.mStories = q.mOutput.mStories;
		mEventClient.notify(StoryDataUpdatedEvent());
	});

	mNodeWatcher.setDelayedMessageNodeCallback([this](const ds::NodeWatcher::Message& m){
		runQueries();
	});

	mWeatherQuery.setReplyHandler([this](WeatherQuery& q) {
		if(q.mWeatherData.mError) {
			mEngine.recordMetric("weather", "errored=1");
			return;
		}
		mAllData.mWeatherData = q.mWeatherData;
		mEngine.getNotifier().notify(WeatherDataUpdatedEvent());
		DS_LOG_INFO("Completed weather query");
		mEngine.recordMetric("weather", "queried=1");
	});

	mEngine.repeatedCallback([this] {runWeatherQuery(); }, 60 * 60); 

	runWeatherQuery();
}

void QueryHandler::runQueries(){
	mStoryQuery.start(nullptr, false);
}

void QueryHandler::onAppEvent(const ds::Event& in_e) {
	// Optionally handle app events to re - query if needed
}

void QueryHandler::runWeatherQuery() {
	mWeatherQuery.start([this](WeatherQuery& q) { q.setInput(mEngine.getAppSettings().getString("wunderground:city"), mEngine.getAppSettings().getString("wunderground:api_key")); });
}

} // !namespace downstream

