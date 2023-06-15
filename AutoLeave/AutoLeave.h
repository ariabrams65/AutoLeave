#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

constexpr int PRIVATE = 6;
constexpr int TOURNAMENT = 22;
constexpr int AUTO_TOURNAMENT = 34;
constexpr int CASUAL_DUEL = 1;
constexpr int CASUAL_DOUBLES = 2;
constexpr int CASUAL_STANDARD = 3;
constexpr int CASUAL_CHAOS = 4;

constexpr float LEAVE_MMR_DELAY = 0.26F;

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
	void exitGame();
	void launchTraining();
	bool isFreeplayMap(const std::string&);
	void hookAll();
	void unhookAll();
	bool shouldQueue(int playlistId);
	bool isCasual(int playlistId);
	bool shouldLeave(int playlistId);
	bool isPrivate(int playlistId);

	void renderCheckbox(const std::string&, const char*);

private:
	bool hooked;

	std::shared_ptr<std::string> trainingMap;
	std::shared_ptr<bool> delayLeaveEnabled;
	std::shared_ptr<bool> casualEnabled;
	std::shared_ptr<bool> queueEnabled;
	std::shared_ptr<bool> launchFreeplayEnabled;
	std::shared_ptr<bool> tournamentsEnabled;
	std::shared_ptr<bool> privateEnabled;
};

