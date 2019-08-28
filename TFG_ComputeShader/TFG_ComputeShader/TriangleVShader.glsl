#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 view; 
uniform mat4 model; 
uniform mat4 projection;
uniform mat4 transform;

out vec3 normal; 

void main()
{
	mat4 MVP = projection * view * model;
	//MVP = mat4(1.0);
	vec4 p = transform * vec4(aPos,1.0);
	gl_Position =  MVP * p;//Posicio amb les transformacions de camera
	normal = aNormal;
}