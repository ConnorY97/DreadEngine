#pragma once
#include "glm.hpp"
#include <string>
#include <vector>
#include "Shader.h"
#include "Texture.h"


using uint = unsigned int;

/*
	Brief: Acts as a vec3 for the Mesh class 
*/
struct Vertex
{	
	Vertex(glm::vec3 new_position)
	{
		position = new_position;
	}
	Vertex(float x, float y, float z, float u, float v)
	{
		position.x = x;
		position.y = y;
		position.z = z;
		tex_coords.x = u;
		tex_coords.y = v;
	}
	Vertex(float x, float y, float z)
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}
	glm::vec3 position;
	glm::vec2 tex_coords;
	//glm::vec3 normal;
};

//struct texture
//{
//	uint id;
//	std::string type; 
//};

class Mesh
{
public:
	//Vertex positions  
	std::vector<Vertex> verticies; 
	//Vertex connection order 
	std::vector<uint> index; 
	//std::vector<texture> textures; 

	/*
		Brief: Builds shapes with the vertex positions and index given
	*/
	Mesh(std::vector<Vertex> verticies, std::vector<uint> index/*, std::vector<texture> textures*/);
	~Mesh(); 

	/*
		Brief: Draws the shapes with a given shader
	*/
	void draw(Shader* shader); 
	void draw(Shader* shader, Texture* texture); 


	//Render Data
	uint VAO, VBO, IBO; 
	
	//Functions
	void set_up_mesh(); 

	
};

