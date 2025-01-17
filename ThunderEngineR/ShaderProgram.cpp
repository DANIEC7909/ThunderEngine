#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	 ViewLoc = glGetUniformLocation(ShaderProgramID, "view");
	 ModelLoc = glGetUniformLocation(ShaderProgramID, "model");
	ProjectionLoc = glGetUniformLocation(ShaderProgramID, "projection");
}

void TE::ShaderProgram::Use()
{
	glUseProgram(ShaderProgramID);
}
//glm::rotate(model, glm::radians(angle), axis)
void TE::ShaderProgram::UpdateMVP(Camera* _camera)
{
	glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, glm::value_ptr(_camera->GetView()));
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniformMatrix4fv(ProjectionLoc, 1, GL_FALSE, glm::value_ptr(_camera->GetProjection()));
}
