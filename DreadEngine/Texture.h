#pragma once
#include <string>
#include "glfw3.h"



class Texture
{
public:
	Texture(std::string texture_name); 

	unsigned int texture; 
};

