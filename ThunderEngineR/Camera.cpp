#include "Camera.h"
#include "TEngine.h"

void Camera::Tick(float deltaTime)
{
	if (glfwGetKey(Camera::EnginePtr->window, GLFW_KEY_W) == GLFW_PRESS)
		Position += cameraForward * deltaTime;//cameraSpeed * cameraFront;
	if (glfwGetKey(Camera::EnginePtr->window, GLFW_KEY_S) == GLFW_PRESS)
		Position -= cameraForward * deltaTime;//cameraSpeed * cameraFront;
	if (glfwGetKey(Camera::EnginePtr->window, GLFW_KEY_A) == GLFW_PRESS)
		Position -= CameraRight * deltaTime;
	if (glfwGetKey(Camera::EnginePtr->window, GLFW_KEY_D) == GLFW_PRESS)
		Position += CameraRight * deltaTime;

	if (glfwGetKey(Camera::EnginePtr->window, GLFW_KEY_E) == GLFW_PRESS)
		Position += glm::vec3(0, 1, 0) * deltaTime;
	if (glfwGetKey(Camera::EnginePtr->window, GLFW_KEY_Q) == GLFW_PRESS)
		Position -= glm::vec3(0, 1, 0) * deltaTime;

	//view = glm::lookAt(/*glm::vec3(front.x,front.y,front.z)*/glm::vec3(Position.x,Position.y,Position.z), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	MouseRot();
	view = glm::lookAt(Position, Position + cameraForward, CameraUp);
	CameraRight = glm::normalize(glm::cross(cameraForward, CameraUp));
	//	CameraUp = glm::normalize(glm::cross(cameraForward, CameraRight));
}

const glm::mat4 Camera::GetView()
{
	return view;
}

const glm::mat4 Camera::GetProjection()
{
	return glm::perspective(glm::radians(Camera::FOV), Camera::aspectRatio, Camera::NearPlane, Camera::FarPlane);

}
glm::vec2 lastMousePos;
void Camera::MouseRot()
{
	glm::vec2 currentMousePos(Camera::EnginePtr->GetCursorPosition().x, Camera::EnginePtr->GetCursorPosition().y);
	glm::vec2 mouseDelta = currentMousePos - lastMousePos;
	lastMousePos = currentMousePos;

	cameraForward = glm::normalize(glm::rotate(cameraForward, glm::radians(mouseDelta.y * CameraSensitivity * (float)Camera::EnginePtr->DeltaTime), glm::vec3(1, 0, 0)));
	cameraForward = glm::normalize(glm::rotate(cameraForward, glm::radians(-mouseDelta.x * CameraSensitivity *(float)Camera::EnginePtr->DeltaTime), glm::vec3(0, 1, 0)));
}
void Camera::Start()
{
	glfwSetCursorPos(Camera::EnginePtr->window, Camera::EnginePtr->screenWidth / 2, Camera::EnginePtr->screenHeight / 2);
}
void Camera::Destroy()
{

}

void Camera::UpdateAspectRatio()
{
	glfwGetFramebufferSize(Camera::EnginePtr->window, &Camera::EnginePtr->screenWidth, &Camera::EnginePtr->screenHeight);
	aspectRatio = static_cast<float>(Camera::EnginePtr->screenWidth) / static_cast<float>(Camera::EnginePtr->screenWidth);
}