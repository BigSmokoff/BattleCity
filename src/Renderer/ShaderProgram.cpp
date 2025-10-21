#include "ShaderProgram.h"
#include <iostream>

namespace Renderer 
{
	// Создаем программу шейдеров
	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLuint vertexShaderID;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
		{
			std::cerr << "VERTEX SHADER compile time error" << std::endl;
			return;
		}

		GLuint fragmentShaderID;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
		{
			std::cerr << "FRAGMENT SHADER compile time error" << std::endl;
			glDeleteShader(vertexShaderID);
			return;
		}

		m_ID = glCreateProgram(); // создаем программу шейдеров
		glAttachShader(m_ID, vertexShaderID); // прикрепляем шейдеры к ней
		glAttachShader(m_ID, fragmentShaderID);
		glLinkProgram(m_ID); // линкуем программу шейдеров

		GLint success;
		GLchar infoLog[1024];
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ID, 1024, nullptr, infoLog); // создаем буфер сообщения
			std::cerr << "ERROR::SHADER: Link time error:\n" << infoLog << std::endl;
		}
	}

	// создаем шейдер
	bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
	{
		shaderID = glCreateShader(shaderType); // создаем идентификатор для шейдера
		const char* code = source.c_str(); // коневртим код c строку
		glShaderSource(shaderID, 1, &code, nullptr); // передаем исходного код шейдера в openGL
		glCompileShader(shaderID); // компилируем шейдер

		GLint success;
		GLchar infoLog[1024];
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success); // проверка компиляции шейдера
		if (!success) 
		{
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog); // создаем буфер сообщения
			std::cerr << "ERROR::SHADER: Compile time error:\n" << infoLog << std::endl;
			return false;
		}
		return true;
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_ID);
	}

	// Берём активную программу шейдера
	void ShaderProgram::use() const
	{
		glUseProgram(m_ID);
	}

	ShaderProgram& ShaderProgram::operator = (ShaderProgram&& shaderProgram) noexcept
	{
		glDeleteProgram(m_ID);
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;

		return *this;
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	{
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
	}
}

/*
[LEARN NOTES]
glShaderSource параметры: 
shaderID - идентификатор шейдера, созданный ранее вызовом
1 - это количество строк, которые ты передаёшь в OpenGL.
&code - это указатель на массив строк (код)
nullptr - это массив длин каждой строки (в символах).

glGetProgramInfoLog парамеры:
m_ID - ID шейдера
1024 - максимальный размер буфера
nullptr - длину реально записанного текста
infoLog - указатель на буфер

std::cerr для ошибок (не буферизирется, можно перенаправить)
noexcept говорит компилятору: эта функция не бросает исключений
*/