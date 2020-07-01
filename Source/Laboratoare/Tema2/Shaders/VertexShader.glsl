#version 330

layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 1) in vec3 v_color;

// Uniform properties
uniform float Time;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// Output value to fragment shader
out vec3 frag_color;
out vec3 frag_normal;

float random( vec2 p )
{
    vec2 K1 = vec2(
        23.14069263277926, // e^pi (Gelfond's constant)
         2.665144142690225 // 2^sqrt(2) (Gelfond–Schneider constant)
    );
    return fract( cos( dot(p,K1) ) * 12345.6789 );
}

void main()
{
	// TODO: send output to fragment shader
	//frag_color = (v_color + vec3(0, sin(Time) - 0.5, cos(Time) - 0.3));
	frag_normal = v_normal;

	vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
    vec3 world_normal = normalize( mat3(Model) * normalize(v_normal) );

	vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 H = normalize(L + V);

	float ambient_light = 0.65;
	float diffuse_light = material_kd * max (dot(N, L), 0);
	float specular_light = 0;

	if (diffuse_light > 0)
		specular_light = material_ks * pow(max(dot(normalize(N), H), 0), material_shininess);

	float light = ambient_light + specular_light + diffuse_light;
	frag_color = v_color * light;
	frag_color = (frag_color + vec3(0, sin(Time)/20.0, sin(Time)/20.0));
	
	float defazajY = 20 * sin(Time * random(vec2(v_position.x, v_position.y)));
	gl_Position = Projection * View * (Model * vec4(v_position, 1.0) + vec4(0,defazajY, 0,0));
	
}
