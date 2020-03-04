//Classic Phong vertex shader
#version 450

layout(location = 0) in vec4 local_position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texture_coordinates;
layout(location = 3) in vec4 tangent; 

out vec4 v_position;
out vec3 v_normal;
out vec2 final_texture_coodinates;
out vec3 v_tangent;
out vec3 v_bitangent; 

uniform mat4 projection_view_matrix;

//Matrix used to transform position
uniform mat4 model_matrix;

//Matrix used to transform normal
uniform mat3 normal_matrix;


void main()
{
	final_texture_coodinates = texture_coordinates;
	v_position = model_matrix * local_position;
	v_normal = normal_matrix * normal.xyz;
	v_tangent = normal_matrix * tangent.xyz;
	v_bitangent = cross(v_normal, v_tangent) * tangent.w;
	//gl_Position = projection_view_matrix * vec4(local_position, 1);
	gl_Position = projection_view_matrix * v_position; 
}