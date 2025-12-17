#pragma once
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>

namespace eng
{
	class Texture;
	class ShaderProgram
	{
	public:
		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator = (const ShaderProgram&) = delete;
		explicit ShaderProgram(unsigned int shaderProgramID);
		~ShaderProgram();

		void Bind();
		int GetUniformLocation(const std::string& name);
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, float v0, float v1);
		void SetUniform(const std::string& name, const glm::mat4& mat);
		void SetUniform(const std::string& name, const glm::vec3& value);
		void SetTexture(const std::string& name, Texture* texture);

	private:
		std::unordered_map<std::string, int> m_uniformLocationCache;
		unsigned int m_shaderProgramID = 0;
		int m_currentTextureUnit = 0;
	};
}
