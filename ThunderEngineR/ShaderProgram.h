#pragma once
#include "TEngine.h"

namespace TE
{
	class ShaderProgram
	{
	public:

		ShaderProgram(const char* FragmentSource, const char* VertexSource);
		void Compile();
		void Use();
	public:
		const char* FragmentSource;
		const char* VertexSource;
		unsigned int ShaderProgramID;
		unsigned int VertexShaderID;
		unsigned int FragmentShaderID;
	};
	
};

