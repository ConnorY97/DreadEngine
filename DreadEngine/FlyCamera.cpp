#include "FlyCamera.h"

void FlyCamera::update(float delta_time)
{
	//Vector to store all the keyboard inputs to 
	glm::vec4 input(0);
	GLFWwindow* window = glfwGetCurrentContext(); 

	//Get the input and copy to vector (result is 1 if key is down)
	input.x += glfwGetKey(window, GLFW_KEY_A); 
	input.x -= glfwGetKey(window, GLFW_KEY_D);
	input.z += glfwGetKey(window, GLFW_KEY_W);
	input.z -= +glfwGetKey(window, GLFW_KEY_S); 

	//Modify the position based off of the input and new speed 
	this->set_position(this->get_world_transform()[3] - (input * speed * delta_time));
}

void FlyCamera::set_speed(float new_speed)
{
	this->speed = new_speed; 
}
