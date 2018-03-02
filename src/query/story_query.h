#pragma  once
#ifndef _LED_SCHMELL_EE_D_APP_QUERY_STORY_QUERY_H_
#define _LED_SCHMELL_EE_D_APP_QUERY_STORY_QUERY_H_

#include <Poco/Runnable.h>

#include "model/all_data.h"

namespace downstream {

class StoryQuery : public Poco::Runnable {
public:
	StoryQuery();

	virtual void			run();

	AllData					mOutput;

private:
	void					query(AllData& output);
};

} // !namespace downstream

#endif //!_LED_SCHMELL_EE_D_APP_QUERY_INDUSTRY_QUERY_H_
