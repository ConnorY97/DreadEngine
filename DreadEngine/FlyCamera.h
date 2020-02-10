#pragma once
#include "Camera.h"
class FlyCamera : public Camera
{
private:
	float speed = 10.0f; 

public:
	void update(float delta_time); 
	
	void set_speed(float new_speed); 
};

