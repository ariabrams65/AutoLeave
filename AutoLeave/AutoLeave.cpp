#include "pch.h"
#include "AutoLeave.h"
#include <sstream>


BAKKESMOD_PLUGIN(AutoLeave, "Automatically leave matches", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void AutoLeave::onLoad()
{
	_globalCvarManager = cvarManager;

	trainingMap = std::make_shared<std::string>("EuroStadium_Night_P");

	registerCvars();
	hookAll();
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

void AutoLeave::onForfeitChanged()
{
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (server.IsNull()) return;
	if (server.GetbCanVoteToForfeit()) return;
	int playlist = server.GetPlaylist().GetPlaylistId();
	if (!(playlist == DUEL || playlist == DOUBLES || playlist == STANDARD || playlist == HOOPS || playlist == RUMBLE || playlist == DROPSHOT || playlist == SNOW_DAY)) return;
	launchTraining();
	gameWrapper->SetTimeout([&](GameWrapper* gw)
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
		LOG("changed");
		*trainingMap = map;
	}
}

void AutoLeave::unhookAll()
{
	gameWrapper->UnhookEvent("Function TAGame.GameEvent_TA.OnCanVoteForfeitChanged");
	gameWrapper->UnhookEvent("Function TAGame.Car_Freeplay_TA.HandleAllAssetsLoaded");
	hooked = false;
}

void AutoLeave::onUnload()
{
}