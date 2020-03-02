#include "Mesh.h"
#include "gl_core_4_5.h"

Mesh::Mesh(std::vector<Vertex> verticies, std::vector<uint> index/*, std::vector<texture> textures*/)
{
	//Assigning the given verticies position
	this->verticies = verticies; 
	//Assigning the given index 
	this->index = index; 
	//this->textures = textures; 

	set_up_mesh(); 
}

Mesh::~Mesh()
{
	//deleting the buffers 
	glDeleteBuffers(1, &IBO); 
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);	
}

Mesh::Mesh(uint vao, uint vbo, uint ibo, uint index_count)
{
	VAO = vao;
	VBO = vbo;
	IBO = ibo; 
	this->index_count = index_count; 
}

void Mesh::set_up_mesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	//A collection of verticies 
	glBindVertexArray(VAO);

	//Positions of the verticies 
	//Asking for free memory on the graphics card 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Filling the memory with data 
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

	//The order in which those verticies are connected 
	//Asking for free memory on the graphics card
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//Filling the memory with data 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(int), &index[0], GL_STATIC_DRAW);


	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	//Normals
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); 
	//Tex-coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);






	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Mesh::draw(Shader* shader)
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, this->index.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::draw(Shader* shader, Texture* texture)
{
	glBindTexture(GL_TEXTURE_2D, texture->texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, this->index.size(), GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::draw(glm::mat4 model, Shader* shader, Texture* texture)
{
	shader->setMat4("model_matrix", model);
	glBindTexture(GL_TEXTURE_2D, texture->texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, this->index.size(), GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Mesh::draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, this->index.size(), GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
