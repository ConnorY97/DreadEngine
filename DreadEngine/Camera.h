#pragma once
#include "glm.hpp"
#include "ext.hpp"
#include "glfw3.h"

class Camera
{
protected:

	//Position of the camera in the world 
	glm::mat4 world_transform; 
	//Settings for what the view from the camera looks like (fov, near plane, far plane)
	glm::mat4 view_transform;
	//Position and rotation of the camera
	glm::mat4 projection_transform;
	//Settings put together
	glm::mat4 projection_view_transform;

	//Recalculates the projection view off the view and the projection
	void update_projection_view_transform();

public:
	//Default constructor
	Camera();

	/*
		Brief: Constructor with default camera view settings 
			param: Field of view of the camera in radians 
			param: Aspect ratio the dimensions of the cameras view 
			param: Near how close the camera is from the near clipping mask 
			param: Far how far the camera is from the far clipping mask 
			
	*/
	Camera(float field_of_view, float apect_ratio, float near, float far);
	
	/*
		Brief: template function to be overloaded with the functionality of the camera 
			param: dela_time the difference in time since the last frame 
	*/

	virtual void update(float delta_time);
	
	//Brief: Sets the position settings for camera 
	void set_perspective(float field_of_view, float aspec_ratio, float near, float far);

	//Sets the world position for the camera 
	void set_look_at(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up);

	void set_position(const glm::vec3& position);

	//Brief: returns the transform for where in world space the camera is positioned
		//Return: glm::mat4&
	const glm::mat4& get_world_transform() { return this->world_transform; };

	//Brief: returns the view of the camera 
	const glm::mat4& get_view() { return this->view_transform; };

	//Brief: Position and rotation of the camera 
	const glm::mat4& get_projection() { return this->projection_transform; };

	//Brief: settings put together, use for the model transform
	const glm::mat4& get_projection_view() { return this->projection_view_transform; };
};

