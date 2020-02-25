#version 450 // Frag Shader

uniform sampler2D diffuse_texture;

in vec2 final_texture_coodinates;
out vec4 final_color;

void main()
{
    vec4 col =  texture(diffuse_texture, final_texture_coodinates);
    if(col.a  < 0.0001)
        discard;
    final_color = col;
}