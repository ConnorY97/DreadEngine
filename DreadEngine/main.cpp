#include <iostream>
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "glm.hpp"
#include "ext.hpp"
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "FlyCamera.h"
#include <vector>
#include "Mesh.h"

using uint = unsigned int; 

void print_shader_error_log(uint shader_id);

int main()
{
	//Initiating the GLFW system
		//Checking if it worked
	if (glfwInit() == false)
		return -1;

	//Creating the window 
	GLFWwindow* pWindow = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	//Creating the camera
	FlyCamera main_camera = FlyCamera(); 

	//Checking if the window was created
	if (pWindow == nullptr)
	{
		glfwTerminate();
		return -2; 
	}
	
	glfwMakeContextCurrent(pWindow);

	//Remap all OpenGL's functions to the correct versions and feature sets
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(pWindow);
		glfwTerminate();
		return -3;
	}

	//Test which version of OpenGl the program is running 
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion(); 
	printf("GL: %i.%i\n", major, minor);

	Shader* pShader = new Shader("../Shaders/simple_vertex.glsl", "../Shaders/simple_color.glsl");

#pragma region Cube
	std::vector<Vertex> cube_verticies
	{
		//First plane 
		Vertex(-0.5f, -0.5f, -0.5f),
		Vertex(-0.5f, 0.5f, -0.5f),
		Vertex(0.5f, 0.5, -0.5f),
		Vertex(0.5f, -0.5f, -0.5f),

		//Second plane 
		Vertex(-0.5f, -0.5f, 0.5f),
		Vertex(-0.5f, 0.5f, 0.5f),
		Vertex(0.5f, 0.5f, 0.5f),
		Vertex(0.5f, -0.5f, 0.5f)
	};

	std::vector<uint> cube_verticies_index
	{
		//Front
		0, 1, 3,
		2, 3, 1,

		//Back
		7, 5, 4,
		5, 7, 6,

		//top 
		2, 1, 5,
		2, 5, 6,

		//Bottom
		4, 0, 3,
		3, 7, 4,

		//Left
		4, 1, 0, 
		1, 4, 5,

		//Right
		7, 3, 2,
		6, 7, 2
	};

	Mesh cube(cube_verticies, cube_verticies_index); 
#pragma endregion

#pragma region Plane
	std::vector<Vertex> plane_verticies
	{
		Vertex(-1.0f, -0.5f, -1.0f),
		Vertex(-1.0f, -0.5f, 1.0f),
		Vertex(1.0f, -0.5f, 1.0f),
		Vertex(1.0f, -0.5f, -1.0f)
	};

	std::vector<uint> plane_verticies_index
	{
		0, 1, 3,
		2, 3, 1
	};

	Mesh plane(plane_verticies, plane_verticies_index); 
#pragma endregion


	

	//Clearing the screen to a specific colour before starting the game loop 
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	//Enable the depth buffer 
	glEnable(GL_DEPTH_TEST);

	glm::mat4 model = glm::mat4(1.0f);

	// Wire-frame mode
	glPolygonMode(GL_BACK, GL_LINE);

	float timer = 0.0f;
	//Game Loop 
		//Checking if the window should close if the Escape key is pressed 
	while (glfwWindowShouldClose(pWindow) == false &&
		glfwGetKey(pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		//Clearing the screen at the start of every loop 
			//COLOR_BUFFER informs OpenGL to wipe the back-buffer colours clean 
			//DEPTH-BUFFER informs it to clear the distance to the closest pixel. To make sure it displays the new image 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		timer += 0.1f;

		main_camera.update(1 / 60.f); 

		model = glm::rotate(model, 0.016f, glm::vec3(0, 1, 0));

		glm::vec4 color = glm::vec4(0.5f); 

		//Turn shader on 
		pShader->Use(); 
		pShader->setMat4("projection_view_matrix", main_camera.get_projection_view());
		pShader->setMat4("model_matrix", model);

		cube.draw(pShader); 
		plane.draw(pShader); 

		//Updating the monitors display by swapping the renderer back buffer 
		glfwSwapBuffers(pWindow);
		//Polling any events sent from the OS
		glfwPollEvents();
	}
	//Code

	//Closing the window and terminating the GLFW system	
	glfwDestroyWindow(pWindow); 
	glfwTerminate(); 
	return 0;
}

void print_shader_error_log(uint shader_id)
{
	// Get the length of the error message
	GLint log_length = 0;
	glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
	// Create the error buffer
	char* log = new char[log_length];
	// Copy the error message
	glGetProgramInfoLog(shader_id, log_length, 0, log);

	// Create the error message
	std::string error_message(log);
	error_message += "SHADER_FAILED_TO_COMPILE";
	printf(error_message.c_str());
	// Clean up anyway
	delete[] log;
}

