#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 1) in vec3 v_color;


// Uniform properties
uniform float Time;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// TODO: output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;

void main()
{
	// TODO: send output to fragment shader
	frag_color = (v_color + vec3(sin(Time) - 0.3, sin(Time) + cos(Time) - 0.3, cos(Time) - 0.3));
	frag_normal = v_normal;

	// TODO: compute gl_Position
	//gl_Position = Projection * View * Model * vec4(v_position, 1.0);
	gl_Position = Projection * View * (Model * vec4(v_position, 1.0) + vec4(sin(Time)*3, cos(Time)*2, cos(Time)*4, 0));
}
