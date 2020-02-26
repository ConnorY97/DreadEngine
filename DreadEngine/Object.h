#pragma once
#include "Mesh.h"
#include "Primitives.h"
#include "Shader.h"
#include "Texture.h"
#include "glm.hpp"
class Object
{
	Object(Shader* shader);
	~Object(); 

	void draw(Shader* shader, Texture* texture); 

	void set_transform(glm::mat4 new_transform); 

private:

	Mesh* object_mesh; 
	
	glm::mat4 object_transform; 
};

