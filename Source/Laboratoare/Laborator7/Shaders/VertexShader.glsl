#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;
uniform float Time;

// Output value to fragment shader
out vec3 color;

void main()
{
	// TODO: compute world space vectors
	vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
    vec3 world_normal = normalize( mat3(Model) * normalize(v_normal) );

	vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 H = normalize(L + V);

	// TODO: define ambient light component
	float ambient_light = 0.35;

	// TODO: compute diffuse light component
	float diffuse_light = material_kd * max (dot(N, L), 0);
	vec3 diffuse_light_vec3 = vec3(Time * diffuse_light, (1 - Time) * diffuse_light ,(1-Time) * diffuse_light);

	// TODO: compute specular light component
	float specular_light = 0;
	vec3 specular_light_vec3;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * pow(max(dot(normalize(N), H), 0), material_shininess);
		specular_light_vec3 = vec3(Time * specular_light, (1 - Time) *specular_light, (1 - Time) *specular_light);
	}

	// TODO: compute light
	float light = ambient_light + specular_light + diffuse_light;

	vec3 light_vec3 = diffuse_light_vec3 +  specular_light_vec3 + vec3(1) * (ambient_light);

	color = object_color * light;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
