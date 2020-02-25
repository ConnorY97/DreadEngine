#include <iostream>
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "Texture.h"
#include "glm.hpp"
#include "ext.hpp"
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "FlyCamera.h"
#include <vector>
#include "Mesh.h"
#include "Primitives.h"
#include "OBJMesh.h"
#include <crtdbg.h>
#include "Material.h"


using uint = unsigned int; 

void print_shader_error_log(uint shader_id);

struct Light
{
	glm::vec3 direction;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

int main()
{
#pragma region Initialisation and checks 
	//At start of main()
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Initiating the GLFW system
		//Checking if it worked
	if (glfwInit() == false)
		return -1;

	//Creating the window 
	GLFWwindow* pWindow = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);
	//Creating the camera
	FlyCamera main_camera = FlyCamera();

	Light light; 

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

#pragma endregion

	Shader* pShader = new Shader("../Shaders/simple_vertex.shader", "../Shaders/simple_frag.shader");

	Mesh* cube = Primitives::cube();
	Mesh* plane = Primitives::plane(); 
	Mesh* sphere = Primitives::sphere(2, 100, 100);

	Texture* test_image = new Texture("../Images/test.jpg"); 
	Texture* baby_yoda = new Texture("../Images/1_mk1-6aYaf_Bes1E3Imhc0A.jpeg");
	Texture* world_map = new Texture("../Images/land_ocean_ice_2048.png");



	aie::OBJMesh* bunbun = new aie::OBJMesh();
	bunbun->load("../Models/stanford/Dragon.obj");

	//Clearing the screen to a specific colour before starting the game loop 
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	//Enable the depth buffer 
	glEnable(GL_DEPTH_TEST);

	glm::mat4 model = glm::mat4(1.0f);
	//glm::vec3 light = glm::vec3(0.0f);

	float total_time = 0;
	glm::vec3 ambient_light; 
	light.diffuse = { 1, 1, 0 };
	light.specular = { 1, 1, 0 };
	ambient_light = { 0.25f, 0.25f, 0.25f }; 

#pragma region Bools
	bool draw_cube = false;
	bool draw_plane = false;
	bool draw_sphere = false;

	bool was_z_down = false;
	bool was_x_down = false;
	bool was_c_down = false;
#pragma endregion

	
	ULONGLONG previous = GetTickCount64(); 
	//Game Loop 
		//Checking if the window should close if the Escape key is pressed 
	while (glfwWindowShouldClose(pWindow) == false &&
		glfwGetKey(pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		//Clearing the screen at the start of every loop 
			//COLOR_BUFFER informs OpenGL to wipe the back-buffer colours clean 
			//DEPTH-BUFFER informs it to clear the distance to the closest pixel. To make sure it displays the new image 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Delta Time
		ULONGLONG now = GetTickCount64();
		float delta_time = float(now - previous) / 1000.0f;
		previous = now; 

		main_camera.update(delta_time);
		total_time += delta_time;

		glm::vec4 color = glm::vec4(0.5f); 
		light.direction = glm::normalize(glm::vec3(glm::cos(total_time * 2), glm::sin(total_time * 2), 0));

		
#pragma region Drawing
		if (glfwGetKey(pWindow, GLFW_KEY_1))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (glfwGetKey(pWindow, GLFW_KEY_2))
			glPolygonMode(GL_FRONT, GL_FILL);
		if (glfwGetKey(pWindow, GLFW_KEY_3))
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		//Turn shader on 
		//pShader->Use();
		//pShader->setMat4("projection_view_matrix", main_camera.get_projection_view());
		//pShader->setMat4("model_matrix", model);
		//pShader->setVec3("light_direction", light);
		//pShader->setVec3("camera_position", glm::vec3(glm::inverse(main_camera.get_view())[3]));
		pShader->Use(); 
		//Set light values 
		pShader->setVec3("Ia", ambient_light);
		pShader->setVec3("Id", light.diffuse);
		pShader->setVec3("Is", light.specular); 
		pShader->setVec3("light_direction", light.direction);
		//Set material values 

		pShader->setVec3("Ka", glm::vec3(bunbun->object_material[0].ambient[0], bunbun->object_material[0].ambient[1], bunbun->object_material[0].ambient[2]));
		pShader->setVec3("Kd", glm::vec3(bunbun->object_material[0].diffuse[0], bunbun->object_material[0].diffuse[1], bunbun->object_material[0].diffuse[2]));
		pShader->setVec3("Ks", glm::vec3(bunbun->object_material[0].specular[0], bunbun->object_material[0].specular[1], bunbun->object_material[0].specular[2]));
		pShader->setFloat("specular_power", bunbun->object_material[0].specularPower); 
		//Bind transform 
		pShader->setMat4("projection_view_matrix", main_camera.get_projection_view()); 
		//Bind transform for lighting 
		pShader->setMat3("normal_matrix", main_camera.get_projection_view()); 
		pShader->setVec3("camera_position", main_camera.get_projection_view()[3]); 


		if (glfwGetKey(pWindow, GLFW_KEY_Z))
		{
			if (!was_z_down)
			{
				was_z_down = true;
				draw_cube = !draw_cube;
			}
		}
		else
			was_z_down = false;
		if (glfwGetKey(pWindow, GLFW_KEY_X))
		{
			if (!was_x_down)
			{
				was_x_down = true;
				draw_plane = !draw_plane;
			}
		}
		else
			was_x_down = false;
		if (glfwGetKey(pWindow, GLFW_KEY_C))
		{
			if (!was_c_down)
			{
				was_c_down = true;
				draw_sphere = !draw_sphere;
			}
		}
		else
			was_c_down = false;
		
		if (draw_cube)
			cube->draw(pShader, test_image);
		if (draw_plane)
			plane->draw(pShader, baby_yoda);
		if (draw_sphere)
			sphere->draw(pShader, world_map);

		bunbun->draw(); 
#pragma endregion
		
		//Updating the monitors display by swapping the renderer back buffer 
		glfwSwapBuffers(pWindow);
		//Polling any events sent from the OS
		glfwPollEvents();
	}
#pragma region Memory cleaning
	//Closing the window and terminating the GLFW system	
	glfwDestroyWindow(pWindow);
	glfwTerminate();
	delete cube;
	cube = nullptr;
	delete plane;
	plane = nullptr;
	delete sphere;
	sphere = nullptr;
	delete pShader;
	pShader = nullptr; 
	delete bunbun;
	bunbun = nullptr;
	delete test_image;
	test_image = nullptr;
	delete baby_yoda;
	baby_yoda = nullptr;
	delete world_map;
	world_map = nullptr; 
#pragma endregion
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

