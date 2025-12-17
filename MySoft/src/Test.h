#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/*--SHADERS STRING--*/
// vertex shader
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"

"out vec3 v_color;\n"
"uniform vec2 u_offset;\n"

"void main()\n"
"{\n"
"	v_color = color;\n"
"	gl_Position = vec4(position.x + u_offset.x, position.y + u_offset.y, position.z, 1.0);\n"
"}\0";

//fragment shader
const char* fragmentShaderSource =
"#version 330 core\n"
"layout (location = 0) out vec4 FragColor;\n"

"in vec3 v_color;\n"
"uniform vec4 u_color;\n"

"void main()\n"
"{\n"
"	FragColor = vec4(v_color, 1.0f) * u_color;\n"
"}\0";

struct Vec2
{
	float x = 0.0f;
	float y = 0.0f;
};

Vec2 offset;

void keyCallBack(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_UP:
			offset.y += 0.1f;
			break;
		case GLFW_KEY_DOWN:
			offset.y -= 0.1f;
			break;
		case GLFW_KEY_LEFT:
			offset.x -= 0.1f;
			break;
		case GLFW_KEY_RIGHT:
			offset.x += 0.1f;
			break;
		default:
			std::cout << "MISINPUT!!" << std::endl;
			break;
		}
	}
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

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int shaderProgram = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	// Error Handling
	int result;
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		// error message
		char message[512];
		glGetShaderInfoLog(shaderProgram, 512, nullptr, message);
		std::cerr << "Failed to link Shaders!" << message << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return shaderProgram;
}

int main()
{
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/*---WINDOW CREATION---*/
	GLFWwindow* window = glfwCreateWindow(800, 600, "MyGame", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Error opening window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	glfwSetKeyCallback(window, keyCallBack);

	float vertices[] = {
		0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
	   -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
	   -0.5f, -0.5f,  0.0f,	0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.0f,	1.0f, 1.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	//VERTEX BUFFER OBJECT
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//INDEX BUFFER OBJECT
	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//VERTEX ARRAY OBJECT
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int shader = CreateShader(vertexShaderSource, fragmentShaderSource);

	int u_ColorLoc = glGetUniformLocation(shader, "u_color");
	int u_OffsetLoc = glGetUniformLocation(shader, "u_offset");

	while (!glfwWindowShouldClose(window))
	{
		// background color
		glClearColor(1.0f, 0.2f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glUniform4f(u_ColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
		glUniform2f(u_OffsetLoc, offset.x, offset.y);

		glBindVertexArray(vao);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}