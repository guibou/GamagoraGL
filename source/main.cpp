#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <vector>
#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <string>
#include "stl.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include "stl.h"

static void error_callback(int /*error*/, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void APIENTRY opengl_error_callback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar *message,
		const void *userParam)
{
	std::cout << message << std::endl;
}

struct TriangleWithNormal
{
    glm::vec3 p0, n0, p1, n1, p2, n2;
};

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if(!gladLoadGL()) {
		std::cerr << "Something went wrong!" << std::endl;
		exit(-1);
	}

	// Callbacks
	glDebugMessageCallback(opengl_error_callback, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	// Shader
	const auto vertex = MakeShader(GL_VERTEX_SHADER, "resources/shaders/shader.vert");
	const auto fragment = MakeShader(GL_FRAGMENT_SHADER, "resources/shaders/shader.frag");

	const auto program = AttachAndLink({vertex, fragment});

	glUseProgram(program);


	// Buffers
	GLuint vbo, vao;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	const auto trisWithoutNormals = ReadStl("logo.stl");

    std::vector<TriangleWithNormal> tris;
    tris.reserve(trisWithoutNormals.size());

    for(int i = 0; i < trisWithoutNormals.size(); i++)
    {
        auto &t = trisWithoutNormals[i];

        glm::vec3 n = glm::normalize(glm::cross(t.p0 - t.p1, t.p0 - t.p2));

        TriangleWithNormal triWithNormal{t.p0, n, t.p1, n, t.p2, n};

        tris.push_back(triWithNormal);
    };

	std::cout << "Nb triangles: " << tris.size() << std::endl;
	const auto nTriangles = tris.size();


	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, nTriangles * sizeof(TriangleWithNormal), tris.data(), GL_STATIC_DRAW);

	// Bindings
	const auto index = glGetAttribLocation(program, "position");

	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), nullptr);
	glEnableVertexAttribArray(index);

	const auto indexNormal = glGetAttribLocation(program, "normal");

	glVertexAttribPointer(indexNormal, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (const void*) sizeof(glm::vec3));
	glEnableVertexAttribArray(indexNormal);

	// glPointSize(20.f);
	//
	glEnable(GL_DEPTH_TEST);

    const auto locTransform = glGetUniformLocation(program, "transformation");
    assert(locTransform != -1);


    const glm::vec3 albedo{0.5, 0.5, 0.5};
    const glm::vec3 Le{10000, 10000, 10000};

    const auto locLightPos = glGetUniformLocation(program, "positionLight");

    const auto locAlbedo = glGetUniformLocation(program, "albedo");

    const auto locLe = glGetUniformLocation(program, "Le");

	while (!glfwWindowShouldClose(window))
	{
        float time = glfwGetTime();
        const glm::vec3 lightPos{50 * std::cos(time), 50 * std::sin(time), 50};

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute the transformation matrix
        const glm::vec3 eye{20, 20, 20};
        glm::mat4 trans = glm::perspective<float>(3.14 / 2, ((float)width) / height, 1.0, 100.0) * glm::lookAt(eye, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});

        glUniformMatrix4fv(locTransform, 1, GL_FALSE, &trans[0][0]);

        glUniform3fv(locLightPos, 1, glm::value_ptr(lightPos));
        glUniform3fv(locAlbedo, 1, glm::value_ptr(albedo));
        glUniform3fv(locLe, 1, glm::value_ptr(Le));


		glDrawArrays(GL_TRIANGLES, 0, nTriangles * 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
