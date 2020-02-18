#include "Primitives.h"

#define PI 3.1415265359f

Mesh* Primitives::cube()
{
	std::vector<Vertex> cube_vertices
	{
		//Back plane 
		/*0*/Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f),	//back bottom left
		/*1*/Vertex(-0.5f, 0.5f, -0.5f,  1.0f, 0.0f),	//back top left
		
		/*2*/Vertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f),		//back top right
		/*3*/Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f),	//back bottom right
		//---------------------------------------------------------------------
		
		//Front plane 
		/*4*/Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f),	//front bottom left
		/*5*/Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f),		//front top left
		
		/*6*/Vertex(0.5f, 0.5f, 0.5f,  1.0f, 0.0f),		//front top right
		/*7*/Vertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f),		//front bottom right
		//---------------------------------------------------------------------

		//Left plane
		/*8*/Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f),	//front bottom left
		/*9*/Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 1.0f),	//back bottom left

		/*10*/Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f),	//front top left
		/*11*/Vertex(-0.5f, 0.5, 0.5f, 1.0f, 0.0f),		//back top left
		//---------------------------------------------------------------------

		//Right plane
		/*12*/Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f),	//front bottom right
		/*13*/Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f),	//back bottom right

		/*14*/Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f),	//front top right
		/*15*/Vertex(0.5f, 0.5, 0.5f, 0.0f, 0.0f),		//back top right
		//---------------------------------------------------------------------
		
		//Top plane 
		/*16*/Vertex(-0.5f, 0.5f, -0.5f,  0.0f, 0.0f),	//back top left
		/*17*/Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f),	//front top left

		/*18*/Vertex(0.5f, 0.5f, 0.5f,  1.0f, 1.0f),	//front top right
		/*19*/Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f),	//back top right
		//---------------------------------------------------------------------

		//Bottom plane
		/*20*/Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f),	//back bottom left
		/*21*/Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f),	//front bottom left

		/*22*/Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f),	//front bottom right
		/*23*/Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f),	//back bottom right
	};

	std::vector<uint> cube_indices
	{
		//Front---------------------------
		0, 1, 3,
		2, 3, 1,

		//Back----------------------------
		7, 5, 4,
		5, 7, 6,
		
		//Left----------------------------
		8, 9, 11,
		10, 8, 11,

		//Right---------------------------
		13, 12, 15,
		14, 15, 12,

		//Top-----------------------------
		16, 17, 19,
		18, 19, 17,

		//Bottom--------------------------
		20, 23, 21,
		22, 21, 23
		
	};


	return new Mesh(cube_vertices, cube_indices);
}

Mesh* Primitives::plane()
{
	std::vector<Vertex> plane_vertices
	{
		Vertex(-0.5f,  0.5f, 0.0f, 1.0f, 0.0f),	//Top Left
		Vertex(0.5f, 0.5f, 0.0f, 0.0f, 0.0f),	//Top right
		Vertex(-0.5f, -0.5f, 0.0f, 1.0f, 1.0f),	//Bottom left
		Vertex(0.5f,  -0.5f, 0.0f, 0.0f, 1.0f)	//Bottom right
	};

	std::vector<uint> plane_indices
	{
		1, 2, 0,
		3, 2, 1
	};
	return new Mesh(plane_vertices, plane_indices);
}

Mesh* Primitives::sphere(float radius, int sector_count, float stack_count)
{	
	std::vector<Vertex> sphere_vertices;
	//std::vector<float> sphere_indeces;
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * PI / sector_count;
	float stackStep = PI / sector_count;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stack_count; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= stack_count; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			sphere_vertices.push_back(Vertex(x, y, z));
			//sphere_vertices.push_back(y);
			//sphere_vertices.push_back(z);

			//// normalized vertex normal (nx, ny, nz)
			//nx = x * lengthInv;
			//ny = y * lengthInv;
			//nz = z * lengthInv;
			//normals.push_back(nx);
			//normals.push_back(ny);
			//normals.push_back(nz);

			//// vertex tex coord (s, t) range between [0, 1]
			//s = (float)j / sectorCount;
			//t = (float)i / stackCount;
			//texCoords.push_back(s);
			//texCoords.push_back(t);
		}
	}

	// generate CCW index list of sphere triangles
	std::vector<uint> sphere_indices;
	int k1, k2;
	for (int i = 0; i < stack_count; ++i)
	{
		k1 = i * (stack_count + 1);     // beginning of current stack
		k2 = k1 + sector_count + 1;      // beginning of next stack

		for (int j = 0; j < sector_count; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				sphere_indices.push_back(k1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stack_count - 1))
			{
				sphere_indices.push_back(k1 + 1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k2 + 1);
			}
		}
	}

	return new Mesh(sphere_vertices, sphere_indices);
}
