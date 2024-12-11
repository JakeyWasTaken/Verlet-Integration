#pragma once
#include "glad/glad.h"
#include "File/File.h"
#include "Config.h"

#define SHADER_ERROR_CHAR_SIZE CONF_MAX_CHAR_SIZE

namespace Verlet
{
	enum ShaderType
	{
		Fragment,
		Vertex,
	};

	namespace ShaderConstructor
	{
		bool CreateAndCompileShader(const char* shaderSource, GLenum shaderType, GLuint& shaderHandle, char* infoLog);

		bool CreateShaderProg(GLuint& shaderProg, GLuint vertexShader, GLuint fragmentShader, char* infoLog);
		void CreateShader(const char* filePath, GLuint& shaderHandle, ShaderType shaderType, bool failFatally);
		
	}
}

