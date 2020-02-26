#include "Object.h"

Object::Object(Shader* shader)
{
	shader->setMat4
}

Object::~Object()
{

}

void Object::draw(Shader* shader, Texture* texture)
{
	object_mesh->draw(shader, texture); 
}

void Object::set_transform(glm::mat4 new_transform)
{
	object_transform = new_transform; 
}