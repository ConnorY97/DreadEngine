#include <iostream>
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "glm.hpp"
#include "ext.hpp"
#include <fstream>
#include <sstream>

using uint = unsigned int; 

int main()
{
	//Initiating the GLFW system
		//Checking if it worked
	if (glfwInit() == false)
		return -1;

	//Creating the window 
	GLFWwindow* pWindow = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

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

	//Create and 'load' a mesh 
	//How many points 
	const uint uiVerticiesSize = 6;
	//Creathing those points 
	glm::vec3 v3Verticies[uiVerticiesSize]
	{
		glm::vec3(-0.5f, 0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f)
	};

	//Vertex array object 
	GLuint VAO;
	//Vertex
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glBufferData(GL_ARRAY_BUFFER, uiVerticiesSize * sizeof(glm::vec3), &v3Verticies[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Clearing the screen to a specific colour before starting the game loop 
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	//Enable the depth buffer 
	glEnable(GL_DEPTH_TEST);

	//Game Loop 
		//Checking if the window should close if the Escape key is pressed 
	while (glfwWindowShouldClose(pWindow) == false &&
		glfwGetKey(pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		//Clearing the screen at the start of every loop 
			//COLOR_BUFFER informs OpenGL to wipe the back-buffer colours clean 
			//DEPTH-BUFFER informs it to clear the distance to the closest pixel. To make sure it displays the new image 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, uiVerticiesSize);

		//Part of the camera
		glm::mat4 view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 pvm = projection * view * model;

		// our game logic and update code goes here!
		// so does our render code!

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

