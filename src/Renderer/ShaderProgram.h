#pragma once

#include <string>
#include <glad//glad.h>

namespace Renderer
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();
		
		bool isCompiled() const { return m_isCompiled; }
		void use() const;

		ShaderProgram() = delete; // запрещаем конструктор без параметров
		ShaderProgram(ShaderProgram&) = delete; // запрещаем копирование
		ShaderProgram& operator = (const ShaderProgram&) = delete; // запрещаем оператор присваивания
		ShaderProgram& operator = (ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

	private:
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
		GLuint m_ID = 0;
		bool m_isCompiled = false;
	};
}

/*
[LEARN NOTES]
- const после метода → не изменяет поля класса
// Буква m = member, то есть переменная-член класса.
*/