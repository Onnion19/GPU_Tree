#version 430 core
#extension GL_NV_gpu_shader5 : enable


layout (local_size_x = 1) in;
 
layout(binding=0) buffer inputBuffer {
    uint16_t input[];
};
 
 layout(binding = 1) buffer brancheBuffer{
	mat4 branches[];
 };

layout(binding=2) buffer outputBuffer {
    mat4 outs[];
};
 
void main()
{
    const uint index =  (gl_WorkGroupID.x) * 16;
	mat4 currentMatrix = mat4(1.0);
	uint troncs = 0; 
	
	
	for(int i = 1; i < 16; i++)
	{
		currentMatrix = branches[ input[index + i] ]* currentMatrix; 
		if( input[index + i] == uint16_t(3) && troncs < 5)
		{
			outs[gl_WorkGroupID.x * 4+troncs] = currentMatrix;			
			troncs ++; 
		}
	}
	
	//const uint index2 = gl_WorkGroupID.x; 
	//outs[index2] = mat4(3);
	
}
		