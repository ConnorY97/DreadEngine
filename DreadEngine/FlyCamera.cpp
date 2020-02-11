#include "FlyCamera.h"

void FlyCamera::update(float delta_time)
{
	//Vector to store all the keyboard inputs to 
	glm::vec4 input(0);
	GLFWwindow* window = glfwGetCurrentContext();

	//Keyboard movement 
	//Get the input and copy to vector (result is 1 if key is down)
	input.x -= glfwGetKey(window, GLFW_KEY_A);
	input.x += glfwGetKey(window, GLFW_KEY_D);
	input.z += glfwGetKey(window, GLFW_KEY_W);
	input.z -= glfwGetKey(window, GLFW_KEY_S);
	input.y += glfwGetKey(window, GLFW_KEY_E);
	input.y -= glfwGetKey(window, GLFW_KEY_Q);

	//Mouse movement 
	double cursor_position_x;
	double cursor_position_y;

	//Get the position of the mouse 
	glfwGetCursorPos(window, &cursor_position_x, &cursor_position_y);

	//Calculate the offset from the screens centre this frame 
	double delta_x = cursor_position_x - (1280 * 0.5);
	double delta_y = cursor_position_y - (720 * 0.5);

	//Move the cursor back to the centre of the window
	glfwSetCursorPos(window, 1280 * 0.5, 720 * 0.5);

	//If either input is non-zero, apply the rotation
	if (delta_x || delta_y)
	{
		//Identity matrix to accumulate rotation
		auto rotation = glm::mat4(1.0f);
		//Left / right rotation 
		rotation = glm::rotate(rotation, float(rot_speed * delta_time * -delta_x), glm::vec3(view_transform[1]));
		//Up / down rotation 
		rotation = glm::rotate(rotation, float(rot_speed * delta_time * -delta_y), glm::vec3(1.0f, 0.0f, 0.0f));

		//Apple the rotation to the camera 
		world_transform = world_transform * rotation;
		//Preserve the inverse 
		//Update PxV
	}

	//Modify the position based off of the input and new speed 
	glm::vec4 move_direction = -input.z * world_transform[2] + input.x * world_transform[0] + input.y * world_transform[1];
	glm::normalize(move_direction);

	this->world_transform[3] += (move_direction * speed * delta_time);
	this->update_projection_view_transform();
	view_transform = glm::inverse(world_transform);
}

void FlyCamera::set_speed(float new_speed)
{
	this->speed = new_speed;
}
