#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Texture::Texture(std::string texture_name)
{
	int x, y, n;
	unsigned char* data = stbi_load(texture_name.c_str(), &x, &y, &n, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Load and generate the texture
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		printf("Failed to load texture!\n"); 
	}
	stbi_image_free(data); 
}
