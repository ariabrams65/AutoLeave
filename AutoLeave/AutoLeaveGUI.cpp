#include "pch.h"
#include "AutoLeave.h"

std::string AutoLeave::GetPluginName() {
	return "AutoLeave";
}

void AutoLeave::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

void AutoLeave::renderCheckbox(const std::string& cvar, const char* desc)
{
	CVarWrapper enableCvar = cvarManager->getCvar(cvar);
	if (!enableCvar) return;
	bool enabled = enableCvar.getBoolValue();
	if (ImGui::Checkbox(desc, &enabled))
	{
		enableCvar.setValue(enabled);
	}
}

void AutoLeave::RenderSettings() {
	renderCheckbox("AutoLeaveEnabled", "Enable plugin");
	ImGui::Separator();
	renderCheckbox("queueEnabled", "Enable auto queue");
	renderCheckbox("launchFreeplayEnabled", "Launch freeplay on game end");
	renderCheckbox("delayLeaveEnabled", "Delay leave in order for MMR to update(goal replays will play at the end of matches)");
	ImGui::Separator();
	renderCheckbox("casualEnabled", "Enable for casual");
	renderCheckbox("tournamentsEnabled", "Enable for tournaments");
	renderCheckbox("privateEnabled", "Enable for private matches and custom tournaments");
}

