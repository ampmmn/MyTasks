#pragma once

#include "AppPreferenceListenerIF.h"
#include "Settings.h"
#include <memory>

class AppPreference
{
	class NotifyWindow;

public:
	static AppPreference* Get();

	void GetProfileDir(CString& dir);
	bool GetFilePath(CString& path);

	void Init();

	void Load();
	void Save();
	void OnExit();

	void SetSettings(const Settings& settings);
	const Settings& GetSettings();


protected:
	AppPreference();
	~AppPreference();

public:
	struct PImpl;
	std::unique_ptr<PImpl> in;

};

