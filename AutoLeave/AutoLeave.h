#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

constexpr int DUEL = 10;
constexpr int DOUBLES = 11;
constexpr int STANDARD = 13;
constexpr int HOOPS = 27;
constexpr int RUMBLE = 28;
constexpr int DROPSHOT = 29;
constexpr int SNOW_DAY = 30;

constexpr float LEAVE_MMR_DELAY = 0.254;

class AutoLeave: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow
{
public:
	virtual void onLoad();
	virtual void onUnload();
	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;

private:
	void registerCvars();
	void cVarEnabledChanged();
	void toggleCvar(const std::string&);
	void onForfeitChanged();
	void onLoadedFreeplay();
	void queue();
	void onStatEvent(void*);
	void leaveMatch();
	void launchTraining();
	bool isFreeplayMap(const std::string&);
	void hookAll();
	void unhookAll();

	void renderCheckbox(const std::string&, const char*);

private:
	bool hooked;
	bool replayActive;

	std::shared_ptr<std::string> trainingMap;
	std::shared_ptr<bool> delayLeaveEnabled;
};

