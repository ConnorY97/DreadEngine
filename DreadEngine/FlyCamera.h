#pragma once
#include "Camera.h"
class FlyCamera : public Camera
{
private:
	float speed = 05.0f; 
	float rot_speed = 0.1f; 

public:
	void update(float delta_time); 
	
	void set_speed(float new_speed); 
};

