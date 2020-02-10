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

	//Remap all OpenGL's fucntions to the correct versions and feature sets
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


#pragma region Geometry

	//Create and 'load' a mesh 
	//How many points 
	//Creating those points 
	const uint verticies_size = 8;
	glm::vec3 verticies[verticies_size]
	{
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		//
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f)
	};
	const uint index_buffer_size = 3 * 2 * 6;
	int index_buffer[index_buffer_size]
	{
		// Back
		0,1,2,
		3,2,1,

		// Front
		6,5,4,
		5,6,7,

		// Bottom
		2,3,6,
		7,6,3,

		// Right
		7,3,1,
		1,5,7,

		// Left 
		4,0,2,
		6,4,2,

		// Top
		1,0,4,
		5,1,4
	};


	//Vertex array object 
	GLuint VAO;
	//Vertex
	GLuint VBO;
	uint IBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	//A collection of verticies 
	glBindVertexArray(VAO);

	//Positions of the verticies 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticies_size * sizeof(glm::vec3), verticies, GL_STATIC_DRAW);

	//The order in which those verticies are connected 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size * sizeof(int), index_buffer, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);



	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Clearing the screen to a specific colour before starting the game loop 
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	//Enable the depth buffer 
	glEnable(GL_DEPTH_TEST);

	glm::mat4 model = glm::mat4(1.0f);
#pragma endregion



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

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, index_buffer_size, GL_UNSIGNED_INT, 0);;

		//Updating the monitors display by swapping the renderer back buffer 
		glfwSwapBuffers(pWindow);
		//Polling any events sent from the OS
		glfwPollEvents();
	}
	//Code

	//Closing the window and terminating the GLFW system
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
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

