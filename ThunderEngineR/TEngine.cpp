#include "TEngine.h"
#include <string>
#include"ShaderProgram.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include "Camera.h"
#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


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
	ImGui_ImplGlfw_InitForOpenGL(window, true);    
	ImGui_ImplOpenGL3_Init();
	glEnable(GL_DEPTH_TEST);
	//TE::TEWindows EngineWindowManager;
	return 0;
}
void CheckOpenGLErrors(const std::string& context) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (err) {
		case GL_INVALID_OPERATION:      error = "GL_INVALID_OPERATION"; break;
		case GL_INVALID_ENUM:           error = "GL_INVALID_ENUM"; break;
		case GL_INVALID_VALUE:          error = "GL_INVALID_VALUE"; break;
		case GL_OUT_OF_MEMORY:          error = "GL_OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
		default:                        error = "UNKNOWN_ERROR"; break;
		}
		std::cerr << "OpenGL Error (" << context << "): " << error << std::endl;
	}
}
void TE::TEngine::MainLoop()
{
    Mesh duckMesh;
    duckMesh.Load("Content/Models/Duck.obj", "Content/Models/Duck.mtl");
    duckMesh.SetupMesh();

    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 view;\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "  gl_Position = projection * view * model*vec4(aPos, 1.0);\n"
        "}\0";

  /*  const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "  FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";*/
    const char* fragmentShaderSource = R"(
#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
  
out vec4 FragColor;

uniform vec3 viewPos; // pozycja kamery
uniform Material material;
uniform Light light;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * material.ambient;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
)";

    ShaderProgram DuckShader = ShaderProgram(fragmentShaderSource, vertexShaderSource);
    DuckShader.Compile();

    Camera camera = Camera(*this);

   

    double lastFrameTime = glfwGetTime();
    std::string WindowFPS = "";

    while (!glfwWindowShouldClose(window)) {
        CheckOpenGLErrors("loop");
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

        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

        camera.UpdateAspectRatio();
        camera.Tick(DeltaTime);
        DuckShader.Use();
        // MATERIAL SETUP  
        //TODO: Make abstraction layer to for sending uniforms.
        glUniform3f(glGetUniformLocation(DuckShader.ShaderProgramID, "material.ambient"), 1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(DuckShader.ShaderProgramID, "material.diffuse"), 1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(DuckShader.ShaderProgramID, "material.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(DuckShader.ShaderProgramID, "material.shininess"), 32.0f);
        CheckOpenGLErrors("material setup");
      
        glUniform3f(glGetUniformLocation(DuckShader.ShaderProgramID, "light.position"), 1.2f, 1.0f, 2.0f);
        glUniform3f(glGetUniformLocation(DuckShader.ShaderProgramID, "light.ambient"), 0.2f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(DuckShader.ShaderProgramID, "light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(DuckShader.ShaderProgramID, "light.specular"), 1.0f, 1.0f, 1.0f);
        CheckOpenGLErrors("loop");
        glUniform3f(glGetUniformLocation(DuckShader.ShaderProgramID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        CheckOpenGLErrors("camera Pos");
        DuckShader.ModelMatrix= glm::rotate(glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)))* glm::rotate(glm::radians((float)(90.0f*glm::sin(glfwGetTime()))), glm::normalize(glm::vec3(0,  0,1)));
         DuckShader.UpdateMVP(&camera);
  
        duckMesh.Draw();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        EngineWindowManager.DrawEngineWindows();

        double curosrPosX, cursorPosY;
        glfwGetCursorPos(window, &curosrPosX, &cursorPosY);
        TEngine::CursorPos.x = curosrPosX;
        TEngine::CursorPos.y = cursorPosY;

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
