#pragma once
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
namespace TE {
	class TEngine;
}
class Camera
{
private:
	float aspectRatio;
private:
	void MouseRot();
public:
	glm::vec3 Position = glm::vec3(0, 0, -3);

	glm::vec3 cameraForward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 CameraRight = glm::vec3(1.0f, 0, 0);

	glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));


	glm::mat4 view;
	float camX;
	float FOV = 45.0f;
	float NearPlane = 0.1f;
	float FarPlane = 100.0f;
	float CameraSensitivity = 0.8f;
	TE::TEngine* EnginePtr;
public:
	Camera(TE::TEngine& _engine)
	{
		EnginePtr = &_engine;
		Start();
	}
	~Camera()
	{
		//if (EnginePtr != nullptr)
		//{
		//	delete EnginePtr;
		//}
		Destroy();
	}
	void Start();
	void Destroy();
	void Tick(float deltaTime);
	float GetAspectRatio() {
		return aspectRatio;
	}
	void UpdateAspectRatio();
	const glm::mat4 GetView();
	const glm::mat4 GetProjection();
};

