#version 400 core

in vec3 normal;
in vec4 pos;
out vec4 FragColor;

void main()
{
	vec3 lightSource = vec3(-150,40,0); 
	float result = dot(lightSource, normal);
	vec3 c = clamp(pos.rgb,vec3(0.0),vec3(1.0));
	FragColor = vec4(c,1.0) * result;
}