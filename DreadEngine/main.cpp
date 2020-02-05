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


	//Load shaders 
	uint uiVertexShadersID = 0;
	uint uiFragmentShaderID = 0; 
	uint uiShaderProgramID = 0; 

	//Vertex shader
		//Getting the vertex shader code from a file 
	std::fstream inFileStream("../Shaders/simple_vertex.glsl", std::ifstream::in); 

	//Writting the code to a string 
	std::string shaderData; 
	std::stringstream stringStream;
	if (inFileStream.is_open())
	{
		stringStream << inFileStream.rdbuf();
		shaderData = stringStream.str();
		inFileStream.close(); 
	}

	//Get free memeory on GPU to write code 
	uiVertexShadersID = glCreateShader(GL_VERTEX_SHADER);
	//Convert string to char 
	const char* shaderCode = shaderData.c_str();
	//Send in the char* to shader
	glShaderSource(uiVertexShadersID, 1, (const GLchar**)&shaderCode, 0);
	//Build
	glCompileShader(uiVertexShadersID);

	//Check if it compiled correctly 
	GLint success = GL_FALSE; 
	glGetShaderiv(uiShaderProgramID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
		printf("Vertex shader failed"); 

	//Fragment shader
	//Same as above for fragment shader
	std::fstream inFileStreamFrag("../Shaders/simple_frag.glsl", std::ifstream::in);

	std::stringstream fragStringStream;
	if (inFileStreamFrag.is_open())
	{
		fragStringStream << inFileStreamFrag.rdbuf();
		shaderData = fragStringStream.str();
		inFileStreamFrag.close(); 
	}

	uiFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	shaderCode = shaderData.c_str(); 
	glShaderSource(uiFragmentShaderID, 1, (const GLchar**)&shaderCode, 0);
	glCompileShader(uiFragmentShaderID);

	//Check
	success = GL_FALSE;
	glGetShaderiv(uiFragmentShaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
		printf("Fragment shader failed"); 

	//Link the shaders
	uiShaderProgramID = glCreateProgram();

	//Attach both shader by ID and type
	glAttachShader(uiShaderProgramID, uiVertexShadersID);
	glAttachShader(uiShaderProgramID, uiFragmentShaderID);

	//Finally link the two pragrams 
	glLinkProgram(uiShaderProgramID);

	//Check if it was successful
	glGetProgramiv(uiShaderProgramID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
		printf("Linking failed"); 	

	
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

		//Part of the camera
		glm::mat4 view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 pvm = projection * view * model;

		glm::vec4 color = glm::vec4(0.5f); 

		//Turn shader on 
		glUseProgram(uiShaderProgramID);

		auto uniformLocation = glGetUniformLocation(uiShaderProgramID, "projectionViewMatrix");
		glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(pvm)); 
		uniformLocation = glGetUniformLocation(uiShaderProgramID, "modelMatrix");
		glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(model));
		uniformLocation = glGetUniformLocation(uiShaderProgramID, "color");
		glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(model));
		glUniform4fv(uniformLocation, 1, glm::value_ptr(color)); 

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, uiVerticiesSize);

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

