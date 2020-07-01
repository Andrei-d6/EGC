#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform sampler2D heightmap;

// Output values to fragment shader
out vec2 texcoord;
out vec3 world_position;
out vec3 world_normal;

void main()
{	
	// TODO: compute world space vertex position and normal
	// TODO: send world position and world normal to Fragment Shader
	float u = v_texture_coord.x;
	float v = v_texture_coord.y;
	texcoord = v_texture_coord;

	vec3 vertex = v_position;
	vertex.y = texture2D(heightmap, v_texture_coord).r * 6.5f;

	world_position = (Model * vec4(vertex, 1)).xyz;
	
	vec2 texelSize = vec2(1.0/256/0, 1.0/256.0);
	float heightRight = texture2D(heightmap, vec2(u + texelSize.x, v)).r;
	float heightUp = texture2D(heightmap, vec2(u, v+texelSize.y)).r;
	float Hx = vertex.y - heightRight;
	float Hz = vertex.y - heightUp;

	world_normal = normalize(vec3(Hx, 1, Hz));
	//world_normal = normalize( mat3(Model) * normalize(v_normal) );

	gl_Position = Projection * View * Model * (vec4(vertex, 1));
}
