//Classic Phong fragent shader
#version 450

in vec4 v_position;
in vec3 v_normal; 
in vec2 final_texture_coodinates;
in vec3 v_tangent;
in vec3 v_bitangent;
out vec4 frag_colour; 

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture; 

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




void main()
{	
	vec4 normal_col = texture(normal_texture, final_texture_coodinates);
	if (normal_col.a < 0.0001)
		discard;

	vec3 textureNormal = normal_col.xyz * 2.0 - 1.0;
	//Ensure normal and light direction are normalised 
	vec3 N = normalize(v_normal);
	vec3 T = normalize(v_tangent);
	vec3 B = normalize(v_bitangent); 
	vec3 L = normalize(light_direction);
	vec3 L2 = normalize(light02_direction);

	mat3 TBN = mat3(T, B, N); 

	vec3 perturbedNormal = TBN * textureNormal;

	//Calculate lambert term (negate light direction)
	float lambert_term1 = max(0, min(1, dot(perturbedNormal, -L)));
	float lmabert_term2 = max(0, min(1, dot(perturbedNormal, -L2)));


	//Calculate view vector and reflection vector
	vec3 V = normalize(camera_position - v_position.xyz);
	vec3 R = reflect(L, perturbedNormal);
	vec3 R2 = reflect(L2, perturbedNormal);

	//Calculate specular term
	float specular_term1 = pow(max(0, dot(R, V)), specular_power); 
	float specular_term2 = pow(max(0, dot(R2, V)), specular_power); 

	//Calculate each colour property
	vec3 ambient = Ia;
	ambient  += Ia2; 
	vec4 texture_colour = texture(diffuse_texture, final_texture_coodinates);
	if (texture_colour.a < 0.0001)
		discard;
	vec3 diffuse = Id * lambert_term1;
	diffuse += Id2 * lmabert_term2;
	diffuse += texture_colour.xyz;
	vec3 specular = Is * specular_term1;
	specular += Is2 * specular_term2; 

	 
	//vec4 final_mat = (texture_colour + normal_col) * 0.5;
	//frag_colour = vec4(ambient + diffuse + specular, 1);
	//frag_colour = vec4(light_direction, 1);
	//frag_colour = vec4(v_normal, 1);
	//frag_colour = (result + diffuse_col);
	//frag_colour = diffuse_col;
	//frag_colour = vec4(final_texture_coodinates, 0, 1);
	//frag_colour = (result + diffuse_col); 
	//frag_colour = vec4(diffuse, 1); 
	
	//vec4 result = vec4(light_direction, 1);
	vec4 result = vec4(diffuse + ambient + specular, 1);
	frag_colour = (result * texture_colour);
}


