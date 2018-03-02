#include "stdafx.h"

#include "led_schmell_ee_d_app.h"

#include <ds/app/engine/engine.h>
#include <ds/ui/interface_xml/interface_xml_importer.h>

#include <ds/ui/media/media_viewer.h>

#include <cinder/app/RendererGl.h>

#include "app/globals.h"
#include "events/app_events.h"

#include "ui/story/story_view.h"
#include "ui/weather/weather_view.h"

namespace downstream {

led_schmell_ee_d_app::led_schmell_ee_d_app()
	: ds::App()
	, mGlobals(mEngine, mAllData)
	, mQueryHandler(mEngine, mAllData)
	, mEventClient(mEngine.getNotifier(), [this](const ds::Event *m){ if(m) this->onAppEvent(*m); })
{

	// Register events so they can be called by string
	// after this registration, you can call the event like the following, or from an interface xml file
	// mEngine.getNotifier().notify("StoryDataUpdatedEvent");
	ds::event::Registry::get().addEventCreator(StoryDataUpdatedEvent::NAME(), [this]()->ds::Event*{return new StoryDataUpdatedEvent(); });
	ds::event::Registry::get().addEventCreator(RequestAppExitEvent::NAME(), [this]()->ds::Event*{return new RequestAppExitEvent(); });

	registerKeyPress("Requery data", [this] { mQueryHandler.runQueries(); }, ci::app::KeyEvent::KEY_n);
}

void led_schmell_ee_d_app::setupServer(){
	mQueryHandler.runQueries();
	
	// add sprites
	mEngine.getRootSprite().addChildPtr(new StoryView(mGlobals));
	mEngine.getRootSprite().addChildPtr(new WeatherView(mGlobals));

	// For this test app, we show the app to start with for simplicity
	// In a real scenario, you'll probably want to start idled / attracting
	mEngine.stopIdling();
}

void led_schmell_ee_d_app::onAppEvent(const ds::Event& in_e){
	if(in_e.mWhat == RequestAppExitEvent::WHAT()){
		quit();
	} 
}

void led_schmell_ee_d_app::fileDrop(ci::app::FileDropEvent event){
	std::vector<std::string> paths;
	for(auto it = event.getFiles().begin(); it < event.getFiles().end(); ++it){
		ds::ui::MediaViewer* mv = new ds::ui::MediaViewer(mEngine, (*it).string(), true);
		mv->initialize();
		mEngine.getRootSprite().addChildPtr(mv);
	}
}

} // namespace downstream

// This line tells Cinder to actually create the application
CINDER_APP(downstream::led_schmell_ee_d_app, ci::app::RendererGl(ci::app::RendererGl::Options().msaa(4)),
		   [&](ci::app::App::Settings* settings){ settings->setBorderless(true); })
