#ifndef _LED_SCHMELL_EE_D_APP_GLOBALS_
#define _LED_SCHMELL_EE_D_APP_GLOBALS_

#include <ds/ui/sprite/sprite_engine.h>

#include "model/all_data.h"

namespace downstream {

/**
 * \class downstream::Globals
 * \brief Global data for the app.
 */
class Globals {
public:
	Globals(ds::ui::SpriteEngine&, const AllData& d);

	ds::ui::SpriteEngine&			mEngine;

	const AllData&					mAllData;

};

} // !namespace downstream

#endif // !_LED_SCHMELL_EE_D_APP_GLOBALS_
