#include "TEngine.h"
#include <string>
#include"ShaderProgram.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"




int TE::TEngine::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "ThunderEngine DEMO", nullptr, nullptr);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//glfwSetKeyCallback(window, keyCallback);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	//init GLEW
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	glViewport(0, 0, screenWidth, screenHeight);

	//imGui stuff
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);    	ImGui_ImplOpenGL3_Init();

	//TE::TEWindows EngineWindowManager;
	return 0;
}

void TE::TEngine::MainLoop()
{
	float vertices[] = {
-0.5f, -0.5f, 0.0f,
 0.5f, -0.5f, 0.0f,
 -0.5f,  0.5f, 0.0f,
 0.5f,  0.5f, 0.0f
	};




	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"  gl_Position =  projection * view * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = " #version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"} ";
	ShaderProgram TriangleShader = ShaderProgram(fragmentShaderSource, vertexShaderSource);
	TriangleShader.Compile();
	TriangleShader.Use();
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	Camera camera = Camera(*this);
	
	
	GLint viewLoc = glGetUniformLocation(TriangleShader.ShaderProgramID, "view");
	GLint modelLoc = glGetUniformLocation(TriangleShader.ShaderProgramID, "model");
	GLint projectionLoc = glGetUniformLocation(TriangleShader.ShaderProgramID, "projection");
	double lastFrameTime = glfwGetTime();
	std::string WindowFPS = "";


	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
		CalcFPS(lastFrameTime);

		WindowFPS = std::to_string(CurrentFPS);
		size_t pos = WindowFPS.find(".");
		if (pos != std::string::npos) {
			if (WindowFPS.size() > pos + 2) {
				WindowFPS.erase(pos + 2);
			}
		}
		std::string fullName;
		fullName = WindowName + " " + WindowFPS;
		glfwSetWindowTitle(window, fullName.c_str());


		// Check if any events have been activiated (key pressed, 
		//mouse moved etc.) and call corresponding response functions 
		glfwPollEvents();
		
		// Render 
		// Clear the colorbuffer 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
		camera.UpdateAspectRatio();
		camera.Tick(DeltaTime);
	
		///Render models Here

	//	glm::mat4 model = glm::mat4(1);
		TriangleShader.Use();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetView()));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.GetProjection()));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 4);

		// Draw OpenGL 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//here draw widows 
		EngineWindowManager.DrawEngineWindows();
		
		double curosrPosX, cursorPosY;
		
			glfwGetCursorPos(window, &curosrPosX, &cursorPosY);

			TE::TEngine::CursorPos.x = curosrPosX;
			TE::TEngine::CursorPos.y = cursorPosY;

		if (ImGui::Begin("Camera Propos")) {
			ImGui::Text(std::to_string(camera.camX).c_str());
			ImGui::Text((std::to_string(TE::TEngine::CursorPos.x) + " mouse x Location").c_str());
		
			
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
	OnDestroy();
}

void TE::TEngine::CalcFPS(double& lastFrameTime)
{
	double currentFrameTime = glfwGetTime();
	DeltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
	CurrentFPS = 1 / DeltaTime;
}

void TE::TEngine::OnDestroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

}
