#include "pch.h"
#include "AutoLeave.h"

std::string AutoLeave::GetPluginName() {
	return "AutoLeave";
}

void AutoLeave::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

void AutoLeave::RenderSettings() {
	CVarWrapper enableCvar = cvarManager->getCvar("AutoLeaveEnabled");
	if (!enableCvar) return;
	bool enabled = enableCvar.getBoolValue();
	if (ImGui::Checkbox("Enable plugin", &enabled))
	{
		enableCvar.setValue(enabled);
	}

	CVarWrapper delayCvar = cvarManager->getCvar("leaveDelay");
	if (!delayCvar) return;
	float delay = delayCvar.getFloatValue();
	if (ImGui::SliderFloat("Delay", &delay, 0, 10))
	{
		delayCvar.setValue(delay);
	}
}

