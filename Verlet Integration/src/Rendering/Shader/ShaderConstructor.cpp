#include "ShaderConstructor.h"
#include "Core/Log.h"

namespace Verlet
{
	bool ShaderConstructor::CreateAndCompileShader(const char* shaderSource, GLenum shaderType, GLuint& shaderHandle, char* infoLog)
	{
		// Construct and compile the shaders
		shaderHandle = glCreateShader(shaderType);
		glShaderSource(shaderHandle, 1, &shaderSource, NULL);
		glCompileShader(shaderHandle);

		// Get state of shader creation
		int success;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);

		// If compilation failed get the error
		if (!success)
		{
			glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
			return false;
		}

		return true;
	}

	bool ShaderConstructor::CreateShaderProg(GLuint& shaderProg, GLuint vertexShader, GLuint fragmentShader, char* infoLog)
	{
		// Create shader prog
		shaderProg = glCreateProgram();

		// Attach shaders and link
		glAttachShader(shaderProg, vertexShader);
		glAttachShader(shaderProg, fragmentShader);
		glLinkProgram(shaderProg);

		// Get program state
		int success;
		glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);

		// If linking failed get the error
		if (!success)
		{
			glGetProgramInfoLog(shaderProg, 512, NULL, infoLog);

			return false;
		}

		// Delete shaders as the program now exists.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return true;
	}

	void ShaderConstructor::CreateShader(const char* filePath, GLuint& shaderHandle, ShaderType shaderType, bool failFatally)
	{
		// Get file and file source
		File file = FileLoader::GetFile(filePath);
		const char* shaderSource = file.FileContents.c_str();

		char infoLog[SHADER_ERROR_CHAR_SIZE];
		uint32_t shaderTypeInt = shaderType == Fragment ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER;

		bool success = CreateAndCompileShader(shaderSource, shaderTypeInt, shaderHandle, infoLog);
		
		// Error state
		if (!success)
		{
			const char* shaderTypeString;
			switch(shaderType)
			{
				case Fragment:
					shaderTypeString = "Fragment";
					break;
				case Vertex:
					shaderTypeString = "Vertex";
					break;
			}
			
			if (failFatally)
			{
				VT_FATAL("Failed to compile {0} shader, file path:\n{1}\nerror:\n{2}", shaderTypeString, filePath, infoLog);
				exit(-1);
			}

			VT_ERROR("Failed to compile {0} shader, file path:\n{1}\nerror:\n{2}", shaderTypeString, filePath, infoLog);
		}
	}
}