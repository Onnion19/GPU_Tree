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
	const uint MAX_TRONCS = 16;
    const uint index =  (gl_WorkGroupID.x) * 16;
	mat4 ScaleMatrix = mat4(0.9); 
	ScaleMatrix[3][3] = 1;
	mat4 currentMatrix = mat4(1.0);
	mat4 lastMatrix = currentMatrix;
	
	
	
	uint troncs = 1; 
	
	int i = 1;
	while(i < 16 && troncs < MAX_TRONCS-1)
	{
		currentMatrix = currentMatrix *branches[ input[index + i] ]; 
		if( input[index + i] == uint16_t(3) || input[index + i] == uint16_t(4))
		{
			currentMatrix = currentMatrix * ScaleMatrix;
			outs[gl_WorkGroupID.x * MAX_TRONCS + troncs] = currentMatrix;
			lastMatrix = currentMatrix;
			troncs ++; 
		}
		i++;
	}
	outs[gl_WorkGroupID.x * MAX_TRONCS] = mat4(troncs - 1);
	
	while(troncs < MAX_TRONCS)//Fill with identity
	{	
		outs[gl_WorkGroupID.x * MAX_TRONCS + troncs] = mat4(0.0);	
		troncs ++;
	}
	
	outs[gl_WorkGroupID.x * MAX_TRONCS + MAX_TRONCS-1 ] = lastMatrix; //Write last matrix 
	
}
		