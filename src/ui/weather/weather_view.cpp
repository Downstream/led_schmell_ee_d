#include "stdafx.h"

#include "weather_view.h"

#include <ds/ui/sprite/sprite_engine.h>
#include <ds/app/engine/engine_events.h>
#include <ds/util/string_util.h>

#include "app/globals.h"
#include "events/app_events.h"


namespace downstream {

WeatherView::WeatherView(Globals& g)
	: ds::ui::SmartLayout(g.mEngine, "weather_view.xml")
	, mGlobals(g)
{

	setSize(mEngine.getWorldWidth(), mEngine.getWorldHeight());

	listenToEvents<WeatherDataUpdatedEvent>([this](const WeatherDataUpdatedEvent& e) { setData(); });

	setData();
}

void WeatherView::setData() {

	auto wd = mGlobals.mAllData.mWeatherData;
	if(wd.mError) return;
	std::string weatherCity = wd.mCity;

	const std::wstring labelSpan = L"";// L"<span weight='bold'>";
	const std::wstring endSpan = L""; // L"</span>";
	const std::wstring celsSpan = L"<span size='xx-small'>";
	const std::wstring celsEndS = L"</span>";
	if(!wd.mDayForecasts.empty()) {
		std::wstringstream labels;
		std::wstringstream content;
		auto df = wd.mDayForecasts.front();


		auto iconny = wd.mCurrentConditions.find("icon");
		if(iconny != wd.mCurrentConditions.end()) {
			/* here's the icon types:
			Chance of Flurries	chanceflurries
			Chance of Rain	chancerain
			Chance Rain	chancerain
			Chance of Freezing Rain	chancesleet
			Chance of Sleet	chancesleet
			Chance of Snow	chancesnow
			Chance of Thunderstorms	chancetstorms
			Chance of a Thunderstorm	chancetstorms
			Clear	clear
			Cloudy	cloudy
			Flurries	flurries
			Fog	fog
			Haze	hazy
			Mostly Cloudy	mostlycloudy
			Mostly Sunny	mostlysunny
			Partly Cloudy	partlycloudy
			Partly Sunny	partlysunny
			Freezing Rain	sleet
			Rain	rain
			Sleet	sleet
			Snow	snow
			Sunny	sunny
			Thunderstorms	tstorms
			Thunderstorm	tstorms
			Unknown	unknown
			Overcast	cloudy
			Scattered Clouds	partlycloudy

			-a----       12/11/2017   8:19 AM           3480 Blizzard.png
			-a----       12/11/2017   8:19 AM           2067 Breezy.png
			-a----       12/11/2017   8:23 AM          18356 Clear.png
			-a----       12/11/2017   8:19 AM           1007 Clear_Night.png
			-a----       12/11/2017   8:19 AM           1556 Drizzle.png
			-a----       12/11/2017   8:19 AM           1430 Fog.png
			-a----       12/11/2017   8:19 AM           1758 Hail.png
			-a----       12/11/2017   8:19 AM           1501 Hurricane.png
			-a----       12/11/2017   8:19 AM           2484 Ice.png
			-a----       12/11/2017   8:19 AM           1321 Mostly_Cloudy.png
			-a----       12/11/2017   8:19 AM           2210 Partly_Cloudy.png
			-a----       12/11/2017   8:19 AM           1720 Partly_Cloudy_Night.png
			-a----       12/11/2017   8:19 AM           1476 rain.png
			-a----       12/11/2017   8:19 AM           2166 Scattered_Showers.png
			-a----       12/11/2017   8:19 AM           1859 Scattered_Showers_Night.png
			-a----       12/11/2017   8:19 AM           2900 sleet.png
			-a----       12/11/2017   8:19 AM           1799 snow.png
			-a----       12/11/2017   8:19 AM           1646 Thunderstorm.png
			*/
			std::string theIcon = iconny->second;
			std::string iconPath = "%APP%/data/images/weather/cloudy.png";
			if(theIcon.find("chancerain") != std::string::npos) {
				iconPath = "%APP%/data/images/weather/Scattered_Showers.png";
			} else if(theIcon.find("rain") != std::string::npos
						) {
				iconPath = "%APP%/data/images/weather/rain.png";
			} else if(theIcon.find("chancesnow") != std::string::npos
						|| theIcon.find("flurries") != std::string::npos
						) {
				iconPath = "%APP%/data/images/weather/snow.png";
			} else if(theIcon.find("snow") != std::string::npos
						) {
				iconPath = "%APP%/data/images/weather/Blizzard.png";
			} else if(theIcon.find("sleet") != std::string::npos
						) {
				iconPath = "%APP%/data/images/weather/sleet.png";
			} else if(theIcon.find("partlycloudy") != std::string::npos
						|| theIcon.find("partlysunny") != std::string::npos
						|| theIcon.find("mostlysunny") != std::string::npos
						) {
				iconPath = "%APP%/data/images/weather/Partly_Cloudy.png";
			} else if(theIcon.find("mostlycloudy") != std::string::npos
						|| theIcon.find("cloudy") != std::string::npos
						) {
				iconPath = "%APP%/data/images/weather/Mostly_Cloudy.png";
			} else if(theIcon.find("sunny") != std::string::npos
						|| theIcon.find("clear") != std::string::npos
						) {
				iconPath = "%APP%/data/images/weather/Clear.png";
			} else if(theIcon.find("chancetstorms") != std::string::npos
						|| theIcon.find("tstorms") != std::string::npos
						) {
				iconPath = "%APP%/data/images/weather/Thunderstorm.png";
			} else if(theIcon.find("fog") != std::string::npos
						|| theIcon.find("hazy") != std::string::npos
						) {
				iconPath = "%APP%/data/images/weather/Fog.png";
			}
			setSpriteImage("weather_icon", iconPath);
		}

		auto locationy = wd.mCurrentConditions.find("full");
		if(locationy != wd.mCurrentConditions.end()) {
			setSpriteText("location_text", locationy->second);
		}

		auto tempStrF = wd.mCurrentConditions.find("temp_f");
		auto tempStrC = wd.mCurrentConditions.find("temp_c");
		auto weatherStr = wd.mCurrentConditions.find("weather");
		if(tempStrF != wd.mCurrentConditions.end()
			&& tempStrC != wd.mCurrentConditions.end()
			&& weatherStr != wd.mCurrentConditions.end()) {

			std::wstringstream fss;
			fss << std::setprecision(0) << roundf(ds::string_to_float(tempStrF->second));
			std::wstringstream css;
			css << std::setprecision(0) << roundf(ds::string_to_float(tempStrC->second));

			std::wstring valuey = fss.str() + L"°F  " + celsSpan + css.str() + L"°C" + celsEndS;

			setSpriteText("now_temp", valuey);
			setSpriteText("now_status", weatherStr->second);
		}

		labels << labelSpan << L"High:" << endSpan << std::endl;
		content << df.mHighTempF << L"  " << celsSpan << df.mHighTempC << celsEndS << std::endl;
		labels << labelSpan << L"Low:" << endSpan << std::endl;
		content << df.mLowTempF << L"  " << celsSpan << df.mLowTempC << celsEndS << std::endl;

		labels << labelSpan << L"Chance of rain:" << endSpan << std::endl;
		content << df.mPrecipChance << std::endl;

		auto windStr = wd.mCurrentConditions.find("wind_string");
		if(windStr != wd.mCurrentConditions.end()) {
			labels << labelSpan << L"Wind:" << endSpan << std::endl;
			content << ds::wstr_from_utf8(windStr->second) << std::endl;
		}

		try {
			if(wd.mDayForecasts.size() > 1) {
				auto tomorrow = wd.mDayForecasts[1];
				labels << std::endl << labelSpan << tomorrow.mWeekday << L": " << endSpan << std::endl;
				content << std::endl << tomorrow.mWeatherText << L" " << tomorrow.mHighTempF << std::endl;
			}
			if(wd.mDayForecasts.size() > 2) {
				auto tomorrow = wd.mDayForecasts[2];
				labels << labelSpan << tomorrow.mWeekday << L": " << endSpan << std::endl;
				content << tomorrow.mWeatherText << L" " << tomorrow.mHighTempF << std::endl;
			}
			if(wd.mDayForecasts.size() > 3) {
				auto tomorrow = wd.mDayForecasts[3];
				labels << labelSpan << tomorrow.mWeekday << L": " << endSpan << std::endl;
				content << tomorrow.mWeatherText << L" " << tomorrow.mHighTempF << std::endl;
			}
		} catch(...) {}


		setSpriteText("labels_text", labels.str());
		setSpriteText("content_text", content.str());
		auto updateStr = wd.mCurrentConditions.find("observation_time");
		if(updateStr != wd.mCurrentConditions.end()) {
			setSpriteText("update_time", updateStr->second);
		}
		
	}

	completeAllTweens(false, true);
	clearAnimateOnTargets(true);
	runLayout();
	tweenAnimateOn(true, 0.0f, 0.05f);
}


void WeatherView::animateOn() {
	show();
	tweenOpacity(1.0f, mEngine.getAnimDur());

	// Recursively animate on any children, including the primary layout
	tweenAnimateOn(true, 0.0f, 0.05f);
}

void WeatherView::animateOff() {
	tweenOpacity(0.0f, mEngine.getAnimDur(), 0.0f, ci::EaseNone(), [this] {hide(); });
}



} // namespace downstream

