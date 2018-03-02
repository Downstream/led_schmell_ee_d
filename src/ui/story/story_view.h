#pragma once
#ifndef _LED_SCHMELL_EE_D_APP_UI_STORY_STORY_VIEW_H_
#define _LED_SCHMELL_EE_D_APP_UI_STORY_STORY_VIEW_H_


#include <ds/ui/sprite/sprite.h>
#include <ds/ui/layout/smart_layout.h>

#include "model/generated/story_model.h"

namespace downstream {

class Globals;

/**
* \class downstream::StoryView
*			A sample view
*/
class StoryView : public ds::ui::SmartLayout  {
public:
	StoryView(Globals& g);

private:
	virtual void						onUpdateServer(const ds::UpdateParams& p) override;

	void								animateOn();
	void								animateOff();

	void								setData();

	Globals&							mGlobals;


};

} // namespace downstream

#endif

