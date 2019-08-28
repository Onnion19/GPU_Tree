#include "ComputeShader.h"


ComputeShader::ComputeShader()
{

}

ComputeShader::ComputeShader(string filepath, bool debug )
{
	_debug = debug;
	Init(filepath);	
}

ComputeShader::ComputeShader(string filepath, unsigned short * branches, int branches_size, glm::mat4 * translate, int translate_size, bool debug)
{
	_buff = buffers(branches, branches_size, translate, translate_size);
	_debug = debug;
	Init(filepath); 
	 
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
	//Generate buffers
		// Input Buffer
			cerr << "Populating input buffer ... " << endl; 
			//for (unsigned int i = 0; i < 32; i++)_buff.inputBuffer[i] = i; 

			cerr << "Input Buffer generating, binding and filling ..." << endl;

			glGenBuffers(1, &_information.inputBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.inputBuffer);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLushort)* _buff.inputBuffer.size(), &_buff.inputBuffer[0], GL_DYNAMIC_DRAW);

			glGenBuffers(1, &_information.branchesBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.branchesBuffer);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4)* _buff.translateBuffer.size(), &_buff.translateBuffer[0], GL_DYNAMIC_DRAW);

		//Output Buffer
			cerr << "Output Buffer generating, binding and filling ..." << endl;
			//glGenBuffers(1, &_information.outputBuffer);
			//glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.outputBuffer);
			//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned)* _buff.outputBuffer.size(), NULL, GL_STREAM_DRAW); //Not sure about GL_STREAM_DRAW, more info at: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
			glGenBuffers(1, &_information.finalBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.finalBuffer);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4)* _buff.finalBuffer.size(), NULL, GL_STREAM_DRAW); //Not sure about GL_STREAM_DRAW, more info at: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	
			cerr << "Choosing Memory Barrier..." << endl;
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // <--Not sure about this one, probably a mistake

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
		if (_debug || true)
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



void ComputeShader::Compute()
{
	if (_debug)
		cerr << "Computing...\nComputing...\nComputing..." << endl;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _information.inputBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _information.branchesBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _information.finalBuffer);

	glUseProgram(_information.programID);

	
	GLint size, workgroup_invocations;
	GLint workgroup_count, workgroup_size;
	glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &size);
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroup_invocations);
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, &workgroup_count);
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_SIZE, &workgroup_size);

	cout << "maxium shared size in MB = " << size/1024 << endl;
	cout << "Maxium invocations in workgroup = " << workgroup_invocations << endl;
	cout << "Maxium compute work = " << workgroup_count << endl;
	cout << "Maxium compute work in group size = " << workgroup_size << endl;

	cout << "Size = " << _buff.inputBuffer.size() << " - MAX WIDHT: " << Config::MAX_WIDTH << endl;
	cout << "Branches send = " << int(_buff.inputBuffer.size() / Config::MAX_WIDTH) << endl;
	glDispatchCompute(int(_buff.inputBuffer.size() / Config::MAX_WIDTH), 1, 1);
	//glDispatchCompute(5, 1, 1);

	// Get the resul0t back //move to output buffer from struct!
	glm::mat4 result[40];
	// Ugly init of the result vector for debug
	for (unsigned int i = 0; i < 40; ++i)
		result[i] = glm::mat4(1.0f);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.finalBuffer);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(result), result);

	cout << "Displaying Results: " << endl; 
	// Get and display the results
	for (unsigned int index = 0; index < 40; ++index)
	{
		
		if (result[index] != glm::mat4(1.0f))
		{
			cerr << "\t Matrix number " << index << " :" << endl;
			for (int i = 0; i < 4; i++)
			{
				cerr << "\t\t    ";
				for (int j = 0; j < 4; j++)
				{
					cerr << result[index][j][i] << "\t\t";
				}
				cerr << endl;
			}
		}
		
	}
	cout << "===================================" << endl; 

	/*PAUSE EXECUTION*/
	cout << "Are the results ok?" << endl; 
	//string a; 
	//cin >> a; 
	/**/
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);//??
}


void ComputeShader::DebugCompute()
{
	
	cerr << "Computing...\nComputing...\nComputing..." << endl;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _information.inputBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _information.outputBuffer);

	glUseProgram(_information.programID);


	GLint size, workgroup_invocations;
	GLint workgroup_count, workgroup_size;
	glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &size);
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroup_invocations);
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, &workgroup_count);
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_SIZE, &workgroup_size);

	cout << "maxium shared size in MB = " << size / 1024 << endl;
	cout << "Maxium invocations in workgroup = " << workgroup_invocations << endl;
	cout << "Maxium compute work = " << workgroup_count << endl;
	cout << "Maxium compute work in group size = " << workgroup_size << endl;
	glDispatchCompute(1, 1, 1);

	// Get the resul0t back //move to output buffer from struct!
	glm::mat4 result[4];
	// Ugly init of the result vector for debug
	for (unsigned int i = 0; i < 4; ++i)
		result[i] = glm::mat4(1.0f);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _information.outputBuffer);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(result), result);

	cout << "Displaying Results: eis " << endl;
	// Get and display the results
	for (unsigned int index = 0; index < 4; ++index)
	{
		cerr << "\t Matrix number " << index << " :" << endl;
		for (int i = 0; i < 4; i++)
		{
			cerr << "\t\t    ";
			for (int j = 0; j < 4; j++)
			{
				cerr << result[index][j][i] << "\t\t";
			}
			cerr << endl;
		}
	}
	cout << "===================================" << endl;

	/*PAUSE EXECUTION*/
	cout << "Are the results ok?" << endl;
	//string a;
	//cin >> a;
	/**/
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);//??
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