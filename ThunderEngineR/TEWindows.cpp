#include "TEWindows.h"
#include "imgui.h";
#include"imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


using namespace TE;

void TEWindows::DrawEngineWindows()
{
	DrawConsole();
}

void TEWindows::DrawConsole()
{
	if (ImGui::Begin("ConsoleWindow")) {
		ImGui::End();
	}
}
