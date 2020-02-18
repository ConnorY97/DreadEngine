#version 450 // Frag Shader

uniform sampler2D diffuse_texture;

in vec2 final_texture_coodinates;
out vec4 final_color;

void main()
{
    final_color = texture(diffuse_texture, final_texture_coodinates);
}