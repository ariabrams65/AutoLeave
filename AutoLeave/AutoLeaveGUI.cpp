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
}

