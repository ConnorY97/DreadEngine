#include "Camera.h"

void Camera::update_projection_view_transform()
{
	this->projection_view_transform = this->projection_transform * this->view_transform; 
}

Camera::Camera()
{
	//Set Defaults 
	this->set_perspective(45/*degrees*/ * 3.14159f / 180/*convert to radians*/, 16 / 9.0f/*wide screen ratio*/, 0.01f, 50.0f); 
	this->set_look_at(glm::vec3(0, 2, 2), glm::vec3(0), glm::vec3(0, 10, 0)); 
}

Camera::Camera(float field_ofview, float apect_ratio, float near, float far)
{
	this->set_perspective(field_ofview, apect_ratio, near, far); 
}

void Camera::update(float delta_time)
{

}

void Camera::set_perspective(float field_of_view, float aspec_ratio, float near, float far)
{
	projection_transform = glm::perspective(field_of_view, aspec_ratio, near, far); 
	this->update_projection_view_transform(); 
}

void Camera::set_look_at(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up)
{
	view_transform = glm::lookAt(from, to, up); 
	//Set world transform to inverse of view_transform because cameras are backwards
	world_transform = glm::inverse(view_transform); 
	this->update_projection_view_transform(); 
}

void Camera::set_position(const glm::vec3& position)
{
	//Get the vector with the positions and sets it 
	world_transform[3] = glm::vec4(position, 1); 
	//Recalculates the view transform based off of the world_transform 
	view_transform = glm::inverse(world_transform); 
	this->update_projection_view_transform(); 
}





