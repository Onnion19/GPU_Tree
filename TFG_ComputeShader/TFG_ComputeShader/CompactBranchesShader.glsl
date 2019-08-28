#version 430 core
#extension GL_NV_gpu_shader5 : enable

layout (local_size_x = 1) in;
 
layout(binding=0) buffer branchBuffer {
    mat4 branch[];
};
 
layout(binding = 1) buffer parentBuffer{
	int16_t parent[];
 };

layout(binding=2) buffer outputBuffer {
    mat4 outs[];
};
 
void main()
{
	const uint BRANCH_OFFSET = 16; 
    const uint index =  (gl_WorkGroupID.x) * BRANCH_OFFSET;
	const uint MAX_BRANCHES = 14;
	
	int indexWrtiteOffset = 0; 	
	for(int i = 0; i < gl_WorkGroupID.x; i++) // calculem on ha de comensar a escriure
	{
		indexWrtiteOffset = indexWrtiteOffset + int(branch[i * BRANCH_OFFSET][0][0]);
	}

	
	mat4 parentTransform = mat4(1.0);
	int nextParent = parent[gl_WorkGroupID.x];
	while(nextParent >= 0) // calculem les transformacions del pare
	{
		parentTransform =  branch[(nextParent+1) * BRANCH_OFFSET - 1] * parentTransform;		
		nextParent = parent[nextParent];
	}
	
	
	int indexBrancaEscriptura = 0; 
	
	while(indexBrancaEscriptura < branch[index][0][0]) 
	{
		outs[indexWrtiteOffset + indexBrancaEscriptura] = parentTransform *branch[index +indexBrancaEscriptura+1]; 
		indexBrancaEscriptura++ ;
	}
	
}
		