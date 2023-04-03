#include "pch.h"
#include "AutoLeave.h"
#include <sstream>


BAKKESMOD_PLUGIN(AutoLeave, "Automatically leave matches", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager; 

void AutoLeave::onLoad() 
{ 
	_globalCvarManager = cvarManager; 
	trainingMap = std::make_shared<std::string>("EuroStadium_Night_P");
	delayLeaveEnabled = std::make_shared<bool>(false);
	casualEnabled = std::make_shared<bool>(true);

	cvarManager->registerNotifier("toggleAutoLeave", [this](std::vector<std::string> args)
		{
			toggleCvar("AutoLeaveEnabled");
		}, "", PERMISSION_ALL);

	registerCvars();
	hookAll();
}

void AutoLeave::toggleCvar(const std::string& cvar)
{
	bool value = cvarManager->getCvar(cvar).getBoolValue();
	cvarManager->executeCommand(cvar + " " + std::to_string(!value));
}

void AutoLeave::registerCvars()
{
	cvarManager->registerCvar("AutoLeaveEnabled", "1")
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar)
			{
				cVarEnabledChanged();
	});
	cvarManager->registerCvar("trainingMap", "EuroStadium_Night_P")
		.bindTo(trainingMap);
	cvarManager->registerCvar("delayLeaveEnabled", "0")
		.bindTo(delayLeaveEnabled);
	cvarManager->registerCvar("casualEnabled", "1")
		.bindTo(casualEnabled);
}

void AutoLeave::cVarEnabledChanged()
{
	bool enabled = cvarManager->getCvar("AutoLeaveEnabled").getBoolValue();
	if (enabled && !hooked)
	{
		hookAll();
	}
	else if (!enabled && hooked)
	{
		unhookAll();
	}
}

void AutoLeave::hookAll()
{
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded",
		[this](std::string eventname)
		{
			onMatchEnded();
		});
	gameWrapper->HookEvent("Function TAGame.GameEvent_TA.OnCanVoteForfeitChanged",
		[this](std::string eventname)
	{
		onForfeitChanged();
	});
	gameWrapper->HookEvent("Function TAGame.Car_Freeplay_TA.HandleAllAssetsLoaded",
		[this](std::string eventname)
	{
		onLoadedFreeplay();
	});
	hooked = true;
}

void AutoLeave::queue()
{
	cvarManager->executeCommand("queue");
}

void AutoLeave::launchTraining()
{
	std::stringstream launchTrainingCommandBuilder;
	launchTrainingCommandBuilder << "start " << *trainingMap << "?Game=TAGame.GameInfo_Tutorial_TA?GameTags=Freeplay";
	gameWrapper->ExecuteUnrealCommand(launchTrainingCommandBuilder.str());
}

void AutoLeave::onMatchEnded()
{
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (server.IsNull()) return;
	int playlist = server.GetPlaylist().GetPlaylistId();
	if (playlist == PRIVATE || playlist == TOURNAMENT) return;
	if (!gameWrapper->GetMMRWrapper().IsRanked(playlist) && !*casualEnabled) return;
	if (gameWrapper->GetMMRWrapper().IsRanked(playlist) && !*delayLeaveEnabled) return;

	queue();
	if (*delayLeaveEnabled)
	{
		gameWrapper->SetTimeout([this](GameWrapper* gw)
			{
				launchTraining();
			}, LEAVE_MMR_DELAY);
	} else 
	{
		launchTraining();
	}
}

void AutoLeave::onForfeitChanged()
{
	if (*delayLeaveEnabled) return;
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (server.IsNull()) return;
	if (server.GetbCanVoteToForfeit()) return;
	int playlist = server.GetPlaylist().GetPlaylistId();
	if (playlist == PRIVATE || playlist == TOURNAMENT) return;
	if (!gameWrapper->GetMMRWrapper().IsRanked(playlist)) return;

	launchTraining();
	gameWrapper->SetTimeout([this](GameWrapper* gw)
		{
			queue();
		}, 0.1);
}

bool AutoLeave::isFreeplayMap(const std::string& map)
{
	if (map.length() < 2) return false;
	return (map[map.length() - 2] == '_' && std::tolower(map[map.length() - 1]) == 'p');
}

void AutoLeave::onLoadedFreeplay()
{
	std::string map = gameWrapper->GetCurrentMap();
	LOG(map);
	if (isFreeplayMap(map))
	{
		*trainingMap = map;
	}
}

void AutoLeave::unhookAll()
{
	gameWrapper->UnhookEvent("Function TAGame.GameEvent_TA.OnCanVoteForfeitChanged");
	gameWrapper->UnhookEvent("Function TAGame.Car_Freeplay_TA.HandleAllAssetsLoaded");
	gameWrapper->UnhookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded");
	hooked = false;
}

void AutoLeave::onUnload()
{
}