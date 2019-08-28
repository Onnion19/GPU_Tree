#include "ComputeShader.h"


ComputeShader::ComputeShader()
{

}

ComputeShader::ComputeShader(string filepath, unsigned int num_tree ,bool debug )
{
	_debug = debug;
	_treeCount = num_tree; 
	if (_treeCount != 0)
		_buff.treeChunksCount.resize(_treeCount, 0);
	Init(filepath);	
}

ComputeShader::ComputeShader(string filepath, unsigned short * branches, int branches_start, int branches_end, glm::mat4 * translate, int translate_size, bool debug)
{
	_debug = debug;
	cerr << "Initalizaing" << endl;
	Init(filepath); 	 
}

vector<glm::mat4> ComputeShader::GetResult()const
{
	return _buff.compactComputedBuffer;
}

void ComputeShader::Init(string filepath)
{

	//Final step
	if (!CompileAndLink(filepath)) // If the compilation goes wrong
	{
		cerr << "Compute Program throwed" << endl;
		string a; 
		cin >> a;
		throw(-15);
	}
	
}

void ComputeShader::BufferSetUpBrancheCompute()
{
	//Generate buffers
		// Input Buffer
	cerr << "Input Buffer generating, binding and filling ..." << endl;

	glGenBuffers(1, &_information.inputBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.inputBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLushort)* _buff.inputBuffer.size(), &_buff.inputBuffer[0], GL_DYNAMIC_DRAW);

	glGenBuffers(1, &_information.translateBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.translateBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4)* _buff.translateBuffer.size(), &_buff.translateBuffer[0], GL_DYNAMIC_DRAW);

		//Output Buffer
	cerr << "Output Buffer generating, binding and filling ..." << endl;
	glGenBuffers(1, &_information.computedBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.computedBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4)* _buff.computedBuffer.size(), NULL, GL_STREAM_DRAW); //Not sure about GL_STREAM_DRAW, more info at: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4)* _buff.num_branches * _buff.max_items_branch, NULL, GL_STREAM_DRAW); //Not sure about GL_STREAM_DRAW, more info at: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml

	cerr << "Choosing Memory Barrier..." << endl;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // <--Not sure about this one, maybe a mistake.

}

void ComputeShader::SendBufferDataAsInt(vector<GLushort> data)
{
	if (!_buff.inputBuffer.empty())
		_buff.inputBuffer.clear();

	_buff.inputBuffer = data;
}


bool ComputeShader::CompileAndLink(string filepath)
{
	_information.programID = glCreateProgram();
	_information.shaderID = glCreateShader(GL_COMPUTE_SHADER);
	if (!_information.shaderID)
	{
		if (_debug)
			cerr << "Error creating shader" << endl;

		return false; 
	}

	const GLchar * shaderProgram = fileToChar(filepath);//Carrega a memòria del shader
	glShaderSource(_information.shaderID, 1, &shaderProgram, NULL);
	glCompileShader(_information.shaderID);

	if (!CheckCompile())
		return false;

	//Creem el programa
	glAttachShader(_information.programID, _information.shaderID);
	glLinkProgram(_information.programID);
	//Make sure GLuseProgram is called at some point before any task ... 

	return true; 
}

bool ComputeShader::CheckCompile()
{
	
	GLint success = 0;
	glGetShaderiv(_information.shaderID, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		cerr << "Checking if it's ok" << endl;
		if (_debug)
		{
			cerr << "FAILED AT COMPILE, RETRIEVING ERROR..." << endl;
			GLint logSize = 0;
			glGetShaderiv(_information.shaderID, GL_INFO_LOG_LENGTH, &logSize);

			std::vector<GLchar> errorLog(logSize);
			glGetShaderInfoLog(_information.shaderID, logSize, &logSize, &errorLog[0]);
			
			cerr << "Displaying shader compiler error" << endl; 
			cerr << "\t";
			for (int i = 0; i < logSize; i++)
			{
				cerr << errorLog[i];
			}
			cerr << endl << endl << endl;
			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(_information.shaderID); // Don't leak the shader.
		}
		return false; 
	}
	else
		return true; 
}


void ComputeShader::SetForBranchCompute(unsigned short * branches, unsigned int branches_start, unsigned int branches_end, glm::mat4 * translate, unsigned int translate_size, bool debug)
{
	_buff = buffers(branches, branches_start, branches_end, translate, translate_size);//Initialize data buffers 
	_debug = debug;
	//Generate Input and ouput OpenGL buffers
	BufferSetUpBrancheCompute();
}

void ComputeShader::Compute() //TODO: CHECK THAT SOMETIMES IT PROCESS ONE EXTRA ITEM IN THE BRANCHES
{
	if (_debug)
		cerr << "Computing...\nComputing...\nComputing..." << endl;

	/***************BUFFER BINDING *************/
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _information.inputBuffer);//Input (L-System) data buffer
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _information.translateBuffer);//Input (L-System to Matrix) conversion buffer
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _information.computedBuffer);//Output Computed Buffer.

	glUseProgram(_information.programID);//Send Shader ID. 

	/*******************COMPUTE****************/
	glDispatchCompute(_buff.num_branches, 1, 1); // Dispatch the Shader

	/**************** GET RESULTS *************/
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.computedBuffer); //OutputBuffer (maybe binded twice, see previous binding) 
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::mat4)*_buff.computedBuffer.size(), &_buff.computedBuffer[0]); //see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetBufferSubData.xhtml
	
	/********** RESULTS POST-PROCESS***************/
	if (_treeCount > 0) //If we have created multiple tree, then find branch hierarchy for every tree
		RetrieveParentsAndTrees();
	else //If dont, just find breanch hierarchy
		RetrieveParents();


	//Not sure if needed
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);//??

	//Release buffers that we no longer need
	//glDeleteBuffers() see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteBuffers.xhtml

	/***************CALL SECOND COMPUTE SHADER PASS********************/
	RecomputeAndCompactBranches();
}


char* ComputeShader::fileToChar(string filepath)const {

	cerr << "Intentant crear el buffer de : " << filepath << endl;
	FILE *file;
	file = fopen(filepath.c_str(), "rb");
	unsigned int length;

	char *buffer = nullptr;
	if (file) {
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		buffer = (char *)malloc(length + 1);
		fseek(file, 0, SEEK_SET);
		fread(buffer, length, 1, file);
		fclose(file);
		buffer[length] = '\0';
		if (_debug)
			cerr << "File open and parsed correctly" << endl;
	}
	else
		cerr << "Ha fallat en obrir el fitxer: " << filepath << endl;

	return buffer;
}


void ComputeShader::RetrieveParents()//RedoForMultipleTree
{
	/* Buffer format: 
	  DEPTH:				       0    1    2    3    4
	  Last seen branch on depth: [   ][   ][   ][   ][   ]
	*/
	short *auxiliar = new short[_buff.num_branches+1]; // Used to update the last branch on the depth level

	for (unsigned int i = 0; i < _buff.num_branches; i++) // init buffer
		auxiliar[i] = -1;

	for (unsigned int branch = 0; branch < _buff.num_branches; branch++)
	{
		unsigned short nivell = _buff.inputBuffer[Config::MAX_WIDTH * branch]; // Get the depth level of the branch from the raw L-System data buffer
		auxiliar[nivell] =  branch; //pdate last branch at that level

		_buff.parentBufferIndices[branch] = auxiliar[nivell - 1];
	}
			
	delete auxiliar; // We no longer need auxiliar buffer!
}


void ComputeShader::RetrieveParentsAndTrees()
{
	RetrieveParents();//Get All hierarchy

	int branch = 0; 
	int tree_num = -1; 
	while (branch < _buff.num_branches) //For each branch
	{
		if (_buff.parentBufferIndices[branch] == -1) //If we don't found a parent branch, that means it's the root == we arrive at the end of the tree
			tree_num++;

		_buff.treeChunksCount[tree_num] += _buff.computedBuffer[Config::MAX_WIDTH * branch][0][0];//Add the number of chunks for the current branch
		branch++;
	}

}


void ComputeShader::SetUpRecomputeAndCompactBranches()
{
	string path = "CompactBranchesShader.glsl";
	Init(path);

	//Buffer Parents indices (input)
	glGenBuffers(1, &_information.parentsBufferIndices);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.parentsBufferIndices);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(short)* _buff.parentBufferIndices.size(), &_buff.parentBufferIndices[0], GL_STREAM_DRAW);

	//Buffer Compact Computed Buffer (output)
	glGenBuffers(1, &_information.compactComputedBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.compactComputedBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4)* _buff.compactComputedBuffer.size(), NULL, GL_STREAM_DRAW);

	cerr << "Choosing Memory Barrier..." << endl;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // <--Not sure about this one, probably a mistake
}

void ComputeShader::RecomputeAndCompactBranches()
{
	SetUpRecomputeAndCompactBranches();//Compile and link the new shader program
	
	if (_debug)
		cerr << "Computing...\nComputing...\nComputing..." << endl;
	/***************BUFFER BINDING *************/
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _information.computedBuffer);//input (output of the previous compute shader) with all the Matrix for every piece of the branch
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _information.parentsBufferIndices);//input indice buffer indicating the branch hierarchy
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _information.compactComputedBuffer);//output Computed buffer

	glUseProgram(_information.programID); //Select Shader

	/*******************COMPUTE****************/	
	glDispatchCompute(_buff.num_branches, 1, 1);//Dispatch shader
	
	/**************** GET RESULTS *************/
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.compactComputedBuffer);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::mat4)*_buff.compactComputedBuffer.size(), &_buff.compactComputedBuffer[0]); // see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetBufferSubData.xhtml

	//Not sure if needed
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);//??

	/********** RESULTS POST-PROCESS***************/

	//Find number of chunks in total
	unsigned int chunks = 0; 
	for (unsigned int i = 0; i < _treeCount; i++)
		chunks += _buff.treeChunksCount[i]; //Find total "chunks" or cubes(mesh) or models

	//Send all cube information to OpenGL 
	rs.SetTransformsRenderSystem(_buff.compactComputedBuffer,chunks); 


	
	/***ROOT FOR EVERY TREE CREATION ****/

	//FURTHER WORK, ALL THIS ALRORITHM MUST BE CONTROLLED BY AN ARTIST, MEANHILE WE CREATE THE ROOTS ON A GRID, AND APPLY SOME JITTER TO SEEM TO BE RANDOM
	srand(time(NULL)); //init seed
	vector<glm::mat4> roots; //Create root buffer
	roots.resize(Config::MAX_TREE, glm::mat4(1.0f));//resize root buffer 
	/*Grid params*/
	int gridsize = 10; // Number of tree for row
	int grid_x = 5; int grid_z = 20; //offset of axes X and Z

	for (int i = 0; i < Config::MAX_TREE; i++) //for each tree
	{

		float offset_x = rand() % gridsize - gridsize/2; //x_jitter
		float offset_z = rand() % gridsize - gridsize/2;//z_jitter

		int posicio_x = i % grid_x; //Calcualte grid position x
		int posicio_z = i / grid_z;//Calcualte grid position z

		//Calculate world position (grid * offset + jitter)
		roots[i][3][0] = posicio_x * gridsize - grid_x /2* gridsize + offset_x;
		roots[i][3][2] = posicio_z * gridsize - grid_z / 10 * gridsize + offset_z;
	}


	/*************SEND DATA TO START RENDERING**************/
	rs.RenderInstancingCubes(_buff.treeChunksCount,roots);

	//Release buffers that we no longer need
	//glDeleteBuffers() see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteBuffers.xhtml
}