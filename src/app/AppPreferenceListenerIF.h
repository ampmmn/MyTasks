#pragma once

class AppPreferenceListenerIF
{
public:
	virtual ~AppPreferenceListenerIF() {}

	virtual void OnAppPreferenceUpdated() = 0;
	virtual void OnAppExit() = 0;
};

