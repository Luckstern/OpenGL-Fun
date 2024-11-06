#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;

//extern "C" {
//	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
//}

const char* vertexShaderSource =
"#version 330 core\n\
layout (location = 0) in vec3 aPos;\n\
layout (location = 1) in vec3 aColor;\n\
out vec3 vertColor;\n\
\n\
void main() {\n\
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n\
	vertColor = aColor;\n\
}";

const char* fragmentShaderSource =
"#version 330 core\n\
in vec3 vertColor;\n\
\n\
void main() {\n\
	gl_FragColor = vec4(vertColor, 0.0f);\n\
}";

float r = 1.0f;
float g = 1.0f;
float b = 1.0f;
unsigned int shaderProgram;
unsigned int VAO;
bool mode;

void changeColor(float& red, float& green, float& blue) {
	red = (float)rand() / RAND_MAX;
	green = (float)rand() / RAND_MAX;
	blue = (float)rand() / RAND_MAX;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	cout << to_string(width) + " " + to_string(height) << endl;
}

//void processInput(GLFWwindow* window) {
//	cout << "processInput was invoked" << endl;
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//		glfwSetWindowShouldClose(window, true);
//	}
//}

void initGL(void) {
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int vertexSuccess;
	int fragmentSuccess;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &fragmentSuccess);
	if (!vertexSuccess) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}
	else if (!fragmentSuccess) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int programSuccess;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);
	if (!programSuccess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void drawTriangle(void) {
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		 0.0,   0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	initGL();

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void drawRectangle(void) {
	float vertices[] = {
		-0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,	 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	initGL();

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		changeColor(r, g, b);
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		if (mode) {
			mode = 0;
			drawTriangle();
		}
		else {
			mode = 1;
			drawRectangle();
		}
		cout << "mode was changed to " << (const char*) (mode ? "rectangle" : "triangle") << endl;
	}
}

int main(void) {
	srand(time(0));
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World!", NULL, NULL);
	if (!window) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);

	cout << "OpenGL " << glGetString(GL_VERSION) << endl;
	cout << "GPU: " << glGetString(GL_RENDERER) << endl;

	drawTriangle();

	mode = 0;
	while (!glfwWindowShouldClose(window)) {
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*float timeVal = glfwGetTime();
		float gVal = (sin(timeVal) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "vertColor");*/
		glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation, 0.0f, gVal, 0.0f, 1.0f);
		glBindVertexArray(VAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mode ? glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) : glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}