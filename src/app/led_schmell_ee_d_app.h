#ifndef _LED_SCHMELL_EE_D_APP_H_
#define _LED_SCHMELL_EE_D_APP_H_

#include <cinder/app/App.h>
#include <ds/app/app.h>
#include <ds/app/event_client.h>

#include "app/globals.h"
#include "query/query_handler.h"

namespace downstream {
class AllData;

class led_schmell_ee_d_app : public ds::App {
public:
	led_schmell_ee_d_app();

	void				setupServer();

	virtual void		fileDrop(ci::app::FileDropEvent event) override;

private:

	void				onAppEvent(const ds::Event&);

	// Data
	AllData				mAllData;

	// Data acquisition
	Globals				mGlobals;
	QueryHandler		mQueryHandler;

	// App events can be handled here
	ds::EventClient		mEventClient;
};

} // !namespace downstream

#endif // !_LED_SCHMELL_EE_D_APP_H_
