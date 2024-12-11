#include "Shader.h"
#include "Core/Log.h"

namespace Verlet
{
	uint32_t ActiveShaderProg;

	Shader::Shader(const char* vertFilePath, const char* fragFilePath)
	{
		m_vertFilePath = vertFilePath;
		m_fragFilePath = fragFilePath;

		CreateShader(false);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_progId);
	}

	void Shader::CreateShader(bool isReloading)
	{
		bool wasActive = ActiveShaderProg == m_progId;

		char infoLog[SHADER_ERROR_CHAR_SIZE];

		ShaderConstructor::CreateShader(m_vertFilePath, m_vertId, Vertex, !isReloading);
		ShaderConstructor::CreateShader(m_fragFilePath, m_fragId, Fragment, !isReloading);

		GLuint bufferedProgId; // Done for hot reloading
		bool success = ShaderConstructor::CreateShaderProg(bufferedProgId, m_vertId, m_fragId, infoLog);

		if (!success)
		{
			if (!isReloading)
			{
				VT_FATAL("Failed to create shader program.\nvert shader:\n{0}\nfrag shader:\n{1}\nerror:\n{2}", m_vertFilePath, m_fragFilePath, infoLog);
				exit(-1);
			}

			VT_ERROR("Failed to create shader program.\nvert shader:\n{0}\nfrag shader:\n{1}\nerror:\n{2}", m_vertFilePath, m_fragFilePath, infoLog);
			return;
		}
		else if (isReloading)
		{
			glDeleteProgram(m_progId);
		}

		VT_TRACE("Created shader with prog id: {0}", bufferedProgId);

		m_progId = bufferedProgId;

		if (wasActive)
			UseShader();
	}

	void Shader::UseShader()
	{
		ActiveShaderProg = m_progId;
		glUseProgram(m_progId);
	}
}