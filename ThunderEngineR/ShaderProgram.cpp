#include "ShaderProgram.h"

TE::ShaderProgram::ShaderProgram(const char* _fragmentSource, const char* _vertexSource)
{
	ShaderProgram::FragmentSource = _fragmentSource;
	ShaderProgram::VertexSource = _vertexSource;
}

void TE::ShaderProgram::Compile()
{
	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderID, 1, &VertexSource, NULL);
	glCompileShader(VertexShaderID);


	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderID, 1, &FragmentSource, NULL);
	glCompileShader(FragmentShaderID);

	ShaderProgramID = glCreateProgram();

	glAttachShader(ShaderProgramID, VertexShaderID);
	glAttachShader(ShaderProgramID, FragmentShaderID);
	glLinkProgram(ShaderProgramID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

void TE::ShaderProgram::Use()
{
	glUseProgram(ShaderProgramID);
}
