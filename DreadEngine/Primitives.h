#pragma once
#include "Mesh.h"
static class Primitives
{
public:
	static Mesh* cube(); 
	static Mesh* plane(); 
	static Mesh* sphere(float radius, int sector_count, float stack_count); 

	//std::vector<Vertex> vertices; 
	//std::vector<uint> indicies; 
};

