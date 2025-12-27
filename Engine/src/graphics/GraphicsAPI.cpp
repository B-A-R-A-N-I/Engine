#include "GraphicsAPI.h"
#include "ShaderProgram.h"
#include "render/Material.h"
#include "render/Mesh.h"
#include <iostream>

namespace eng
{
	bool GraphicsAPI::Init()
	{
		glEnable(GL_DEPTH_TEST);
		return true;
	}

	static unsigned int CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		// Error Handling
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			// error message
			char message[512];
			glGetShaderInfoLog(id, 512, nullptr, message);
			std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader!" << std::endl;
			std::cerr << message << std::endl;
		}
		return id;
	}

	std::shared_ptr<ShaderProgram> GraphicsAPI::CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
	{
		unsigned int shaderProgramID = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

		glAttachShader(shaderProgramID, vs);
		glAttachShader(shaderProgramID, fs);
		glLinkProgram(shaderProgramID);
		glValidateProgram(shaderProgramID);

		// Error Handling
		int result;
		glGetShaderiv(shaderProgramID, GL_LINK_STATUS, &result);
		if (!result)
		{
			// error message
			char message[512];
			glGetShaderInfoLog(shaderProgramID, 512, nullptr, message);
			std::cerr << "Failed to link Shaders!" << message << std::endl;
			return nullptr;
		}

		glDeleteShader(vs);
		glDeleteShader(fs);

		return std::make_shared<ShaderProgram>(shaderProgramID);
	}

	const std::shared_ptr<ShaderProgram>& GraphicsAPI::GetDefaultShaderProgram()
	{
		if (!m_defaultShaderProgram)
		{
			std::string vertexShaderSource = R"(
			#version 330 core

			layout(location = 0) in vec3 position;
			layout(location = 1) in vec3 color;
			layout(location = 2) in vec2 uv;
			layout(location = 3) in vec3 normal;

			out vec2 vUV;
			out vec3 vNormal;
			out vec3 vFragPos;

			uniform mat4 uModel;
			uniform mat4 uView;
			uniform mat4 uProjection;

			void main()
			{
				vUV = uv;

				vFragPos = vec3(uModel * vec4(position, 1.0));

				vNormal = mat3(transpose(inverse(uModel))) * normal;

				gl_Position = uProjection * uView * uModel * vec4(position, 1.0);
			}
			)";

			std::string fragmentShaderSource = R"(
			#version 330 core

			struct Light
			{
				vec3 color;
				vec3 direction;
			};

			uniform Light uLight;
			uniform vec3 uCameraPos;

			out vec4 FragColor;

			in vec2 vUV;
			in vec3 vNormal;
			in vec3 vFragPos;

			uniform sampler2D baseColorTexture;

			void main()
			{
				vec3 norm = normalize(vNormal);

				
				// diffuse
				vec3 lightDir = normalize(-uLight.direction);
				float diff = max(dot(norm, lightDir), 0.0);
				vec3 diffuse = diff * uLight.color;

				// specular
				vec3 viewDir = normalize(uCameraPos - vFragPos);
				vec3 redlectDir = reflect(-lightDir, norm);
				float spec = pow(max(dot(viewDir, redlectDir), 0.0), 32.0);
				float specularStrength = 0.5;
				vec3 specular = specularStrength * spec * uLight.color;
				
				// ambient
				const float ambientStrength = 0.4;
				vec3 ambient = ambientStrength * uLight.color;

				vec3 result = diffuse + specular + ambient;

				vec4 texColor = texture(baseColorTexture, vUV);

				FragColor = texColor * vec4(result, 1.0);
			}
			)";

			m_defaultShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
		}

		return m_defaultShaderProgram;
	}

	unsigned int GraphicsAPI::CreateVertexBuffer(const std::vector<float>& vertices)
	{
		//VERTEX BUFFER OBJECT
		unsigned int VBO = 0;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return VBO;
	}

	unsigned int GraphicsAPI::CreateIndexBuffer(const std::vector<uint32_t>& indices)
	{
		//INDEX BUFFER OBJECT
		unsigned int EBO = 0;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return EBO;
	}

	void GraphicsAPI::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void GraphicsAPI::ClearBuffers()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GraphicsAPI::BindShaderProgram(ShaderProgram* shaderProgram)
	{
		if (shaderProgram)
		{
			shaderProgram->Bind();
		}
	}

	void GraphicsAPI::BindMaterial(Material* material)
	{
		if (material)
		{
			material->Bind();
		}
	}

	void GraphicsAPI::BindMesh(Mesh* mesh)
	{
		if (mesh)
		{
			mesh->Bind();
		}
	}

	void GraphicsAPI::UnbindMesh(Mesh* mesh)
	{
		if (mesh)
		{
			mesh->Unbind();
		}
	}

	void GraphicsAPI::DrawMesh(Mesh* mesh)
	{
		if (mesh)
		{
			mesh->Draw();
		}
	}
}