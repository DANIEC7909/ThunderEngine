#pragma once
#include "TEngine.h"
#include "Camera.h"

namespace TE
{
	class ShaderProgram
	{
	public:

		ShaderProgram(const char* FragmentSource, const char* VertexSource);
		void Compile();
		void Use();
		void UpdateMVP( Camera* _camera);
	public:
		const char* FragmentSource;
		const char* VertexSource;
		unsigned int ShaderProgramID;
		unsigned int VertexShaderID;
		unsigned int FragmentShaderID;

		unsigned int ProjectionLoc, ViewLoc, ModelLoc;
		glm::mat4 ModelMatrix;
	};
	
};

