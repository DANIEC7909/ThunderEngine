
#include "imgui.h";
#include"imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream> 

#include "TEngine.h"
// GLEW 
#define GLEW_STATIC 
#include <GL/glew.h> 

// GLFW 
#include <GLFW/glfw3.h> 

int main()
{
	TE::TEngine Engine;
	Engine.Init();
	Engine.MainLoop();
	return EXIT_SUCCESS;

}

