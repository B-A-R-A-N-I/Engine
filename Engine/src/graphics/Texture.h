#pragma once
#include <glad/glad.h>
#include <memory>
#include <string>

namespace eng
{
	class Texture
	{
	public:
		Texture(int width, int height, int numChannels, unsigned char* data);
		~Texture();
		unsigned int GetID() const;
		void Init(int width, int height, int numChannels, unsigned char* data);

		static std::shared_ptr<Texture> Load(const std::string path);

	private:
		int m_width = 0;
		int m_height = 0;
		int m_numChannels = 0;
		unsigned int m_textureID = 0;
	};
}