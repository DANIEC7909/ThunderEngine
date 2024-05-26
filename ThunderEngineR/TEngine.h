#pragma once
#include "imgui.h"
#include"imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream> 
// GLEW 
#define GLEW_STATIC 
#include<GL/glew.h> 
 

// GLFW 
#include <GLFW/glfw3.h>
//Engine classes.
#include "TEWindows.h"
#include <glm/vec2.hpp>


namespace TE
{
	class TEngine
	{
	private:
		const GLint WIDTH = 1920, HEIGHT = 1080
;
		glm::vec2 CursorPos;

	public:
		GLFWwindow* window;
		GLint screenWidth, screenHeight;
		TEWindows EngineWindowManager;
		double DeltaTime;
		double CurrentFPS;
		std::string WindowName = "ThunderEngine DEMO";
		float spectRatio;
	public:
		int Init();
		void MainLoop();
		void CalcFPS(double& lastFrameTime);
		void OnDestroy();
		const glm::vec2 GetCursorPosition()
		{
			return CursorPos;
		}

	};
}
