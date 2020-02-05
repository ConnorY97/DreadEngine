#include <iostream>
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "glm.hpp"
#include "ext.hpp"
#include <fstream>
#include <sstream>
#include "Shader.h"

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

	Shader* pShader = new Shader("../Shaders/simple_vertex.glsl", "../Shaders/simple_color.glsl");


	
	//Create and 'load' a mesh 
	//How many points 
	//const uint uiVerticiesSize = 8;
	//Creathing those points 
	const uint verticies_size = 8;
	glm::vec3 verticies[verticies_size]
	{
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),

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
		1,2,3,

		// Front
		4,5,6,
		5,6,7,

		// Bottom
		2,3,6,
		3,6,7,

		// Right
		1,3,7,
		1,5,7,

		// Left 
		2,0,4,
		2,4,6,

		// Top
		4,0,1,
		4,1,5
	};

	//Vertex array object 
	GLuint VAO;
	//Vertex
	GLuint VBO;
	uint IBO; 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO); 

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticies_size * sizeof(glm::vec3), verticies, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size * sizeof(int), index_buffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glBufferData(GL_ARRAY_BUFFER, verticies_size * sizeof(glm::vec3), &verticies[0], GL_STATIC_DRAW);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Clearing the screen to a specific colour before starting the game loop 
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	//Enable the depth buffer 
	glEnable(GL_DEPTH_TEST);

	glm::mat4 model = glm::mat4(1.0f);


	// Wire-frame mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Game Loop 
		//Checking if the window should close if the Escape key is pressed 
	while (glfwWindowShouldClose(pWindow) == false &&
		glfwGetKey(pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		//Clearing the screen at the start of every loop 
			//COLOR_BUFFER informs OpenGL to wipe the back-buffer colours clean 
			//DEPTH-BUFFER informs it to clear the distance to the closest pixel. To make sure it displays the new image 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Part of the camera
		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0), glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

		model = glm::rotate(model, 0.016f, glm::vec3(0, 1, 0));

		glm::mat4 pvm = projection * view * model;

		glm::vec4 color = glm::vec4(0.5f); 

		//Turn shader on 
		//glUseProgram(uiShaderProgramID);
		pShader->Use(); 
		pShader->setMat4("projection_view_matrix", pvm);
		pShader->setMat4("model_matrix", model);
		pShader->setVec4("color", color); 

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, index_buffer_size, GL_UNSIGNED_INT, 0);;

		// our game logic and update code goes here!
		// so does our render code!

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

