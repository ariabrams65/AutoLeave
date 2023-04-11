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
	renderCheckbox("delayLeaveEnabled", "Delay leave in order for MMR to update(goal replays will play at the end of matches)");
	renderCheckbox("casualEnabled", "Enable for casual");
	renderCheckbox("queueEnabled", "Enable auto queue");
	renderCheckbox("launchFreeplayEnabled", "Launch freeplay on game end");
}

