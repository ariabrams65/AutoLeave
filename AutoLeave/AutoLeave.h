#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

constexpr int PRIVATE = 6;
constexpr int TOURNAMENT = 22;

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
	void onMatchEnded();
	void onLoadedFreeplay();
	void queue();
	void launchTraining();
	bool isFreeplayMap(const std::string&);
	void hookAll();
	void unhookAll();

	void renderCheckbox(const std::string&, const char*);

private:
	bool hooked;

	std::shared_ptr<std::string> trainingMap;
	std::shared_ptr<bool> delayLeaveEnabled;
	std::shared_ptr<bool> casualEnabled;
	std::shared_ptr<bool> queueEnabled;
};

