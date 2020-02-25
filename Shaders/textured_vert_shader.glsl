#version 450 // Vert shader

layout(location = 0) in vec3 local_position;
layout(location = 2) in vec2 texture_coordinates;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;

out vec2 final_texture_coodinates;

void main()
{
    final_texture_coodinates = texture_coordinates;
    gl_Position = (projection_view_matrix * model_matrix) * vec4(local_position, 1);
}