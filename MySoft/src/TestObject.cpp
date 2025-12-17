#include "TestObject.h"
#include "GLFW/glfw3.h"
#include <iostream>

TestObject::TestObject()
{
	/*--SHADERS STRING--*/
	// vertex shader
	std::string vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 position;
		layout (location = 1) in vec3 color;

		out vec3 vColor;

		uniform mat4 uModel;
		uniform mat4 uView;
		uniform mat4 uProjection;

		void main()
		{
			vColor = color;
			gl_Position =  uProjection * uView * uModel * vec4(position, 1.0);
		}
	)";

	//fragment shader
	std::string fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;

		in vec3 vColor;

		void main()
		{
			FragColor = vec4(vColor, 1.0f);
		}
	)";

	auto& graphicsAPI = eng::Engine::GetInstance().GetGraphicsAPI();
	auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
	
	auto material = std::make_shared<eng::Material>();
	material->SetShaderProgram(shaderProgram);

	std::vector<float> vertices =
	{
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
	   -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
	   -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f,-0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

		0.5f, 0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
	   -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
	   -0.5f,-0.5f,-0.5f, 0.0f, 0.0f, 1.0f,
		0.5f,-0.5f,-0.5f, 1.0f, 1.0f, 0.0f
	};

	std::vector<unsigned int> indices =
	{
		// front face
		0, 1, 2,
		0, 3, 2,
		// left face
		1, 2, 6,
		1, 5, 6,
		// bottom face
		2, 3, 7,
		2, 6, 7,
		// right face
		3, 0, 4,
		3, 7, 4,
		// top face
		4, 0, 1,
		4, 5, 1,
		// back face
		5, 4, 7,
		5, 6, 7
	};

	eng::VertexLayout vertexLayout;

	//Position
	vertexLayout.elements.push_back({
		0,
		3,
		GL_FLOAT,
		0
		});

	//Color
	vertexLayout.elements.push_back({
		1,
		3,
		GL_FLOAT,
		sizeof(float) * 3
		});

	vertexLayout.stride = sizeof(float) * 6;

	auto mesh = std::make_shared<eng::Mesh>(vertexLayout, vertices, indices);

	AddComponent(new eng::MeshComponent(material, mesh));
}

void TestObject::Update(float deltaTime)
{
	eng::GameObject::Update(deltaTime);

#if 0
	auto position = GetPosition();
	auto& input = eng::Engine::GetInstance().GetInputManager();

	// horizontal movement
	if (input.IsKeyPressed(GLFW_KEY_A))
	{
		position.x -= 0.001f;
	}
	else if (input.IsKeyPressed(GLFW_KEY_D))
	{
		position.x += 0.001f;
	}

	// vertical movement
	if (input.IsKeyPressed(GLFW_KEY_S))
	{
		position.y -= 0.001f;
	}
	else if (input.IsKeyPressed(GLFW_KEY_W))
	{
		position.y += 0.001f;
	}
	SetPosition(position);
#endif
}