#pragma once

#include "AppPreferenceListenerIF.h"
#include "Settings.h"
#include "core/Section.h"
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

	void SetSections(const std::vector<Section>& sections);
	void GetSections(std::vector<Section>& sections);

protected:
	AppPreference();
	~AppPreference();

public:
	struct PImpl;
	std::unique_ptr<PImpl> in;

};

