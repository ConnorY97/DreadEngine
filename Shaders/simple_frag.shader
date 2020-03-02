//Classic Phong fragent shader
#version 450

in vec4 v_position;
in vec3 v_normal; 

uniform sampler2D diffuse_texture;

uniform vec3 Ka;				//Ambient mat colour
uniform vec3 Kd;				//Diffuse mat colour
uniform vec3 Ks;				//Specular mat colour
uniform float specular_power;	//Mat specular power

//First light 
uniform vec3 Ia;				//Ambient light colour
uniform vec3 Id;				//Diffuse light colour
uniform vec3 Is;				//Specular light colour 
uniform vec3 light_direction;

//Second light 
uniform vec3 Ia2;
uniform vec3 Id2;
uniform vec3 Is2;
uniform vec3 light02_direction; 

uniform vec3 camera_position; 

in vec2 final_texture_coodinates;
out vec4 frag_colour; 



void main()
{	
	//Ensure normal and light direction are normalised 
	vec3 N = normalize(-v_normal);
	vec3 L = normalize(light_direction);
	vec3 L2 = normalize(light02_direction);

	//Calculate lambert term (negate light direction)
	float lambert_term1 = max(0, min(1, dot(N, -L))); 
	float lmabert_term2 = max(0, min(1, dot(N, -L2))); 


	//Calculate view vector and reflection vector
	vec3 V = normalize(camera_position - v_position.xyz);
	vec3 R = reflect(L, N);
	vec3 R2 = reflect(L2, N); 

	//Calculate specular term
	float specular_term1 = pow(max(0, dot(R, V)), specular_power); 
	float specular_term2 = pow(max(0, dot(R2, V)), specular_power); 

	//Calculate each colour property
	vec3 ambient = Ia * Ka * Ia2;
	ambient += Ia2 * Ka; 
	vec3 diffuse = Id * Kd * lambert_term1;
	diffuse += Id2 * lmabert_term2;
	vec3 specular = Is * Ks * specular_term1;
	specular += Is2 * specular_term2; 

	vec4 col = texture(diffuse_texture, final_texture_coodinates);
	if (col.a < 0.0001)
		discard;
	//frag_colour = vec4(ambient + diffuse + specular, 1);
	//frag_colour = vec4(light_direction, 1);
	//frag_colour = vec4(v_normal, 1);
	vec4 result = vec4(ambient + diffuse + specular, 1); 
	frag_colour = (result + col);
	//frag_colour = vec4(final_texture_coodinates, 0, 1);
}


