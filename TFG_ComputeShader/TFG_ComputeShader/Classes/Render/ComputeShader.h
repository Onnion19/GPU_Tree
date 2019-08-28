#include <iostream>

#include "../LSystem/Config.h"
#include "renderSystem.h"
// System Headers
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
/**
* Struct for Gluints info. 
* Stores all the Gluint info of the compute shader. It is composed by: 1 inputbuffer, 1 output buffer, compiled program id, and shader id 
*/
struct gluintsInfo
{
	GLuint shaderID; //Informació del shader
	GLuint programID; //Complied ID program
	GLuint inputBuffer;//Input Buffer
	GLuint branchesBuffer;//Branches buffer
	GLuint outputBuffer; //Output Buffer
	GLuint finalBuffer;
	gluintsInfo() {
		shaderID = programID = inputBuffer = outputBuffer = 0; 
	}
	
};
/**
* Struct for shader buffers. 
* Stores all the buffer data. In only can hold two buffers, (input and output). Default constructor creates an input buffer of 8 elements initialized at 1, and 
* an ouput buffer of 8 elements initialized at 0.
*/
struct buffers
{
	vector< GLushort > inputBuffer;
	vector < glm::mat4 > translateBuffer; 
	vector< unsigned int > outputBuffer;
	vector< glm::mat4 > finalBuffer; 
	
	//unsigned int c_buff[16][2];
	/**
	* Default constructor. 
	* Initializes input buffer at: 32 elements, value 1. And output buffer at: 32 elements, value 0.
	*/
	buffers() { inputBuffer.resize(8, 1); outputBuffer.resize(8, 0); }
	/**
	* Parametric Constructor. 
	* Initializes input buffer at: <input_size>, value <input_init>. And output buffer at: <output_size>, value <output_init>. 
	* @param <unsigned> input_size: size of the input vector. 
	* @param <unsigned> input_init: value of the elements of input vector. 
	* @param <unsigned> output_size: size of the output vector.
	* @param <unsigned> output_init: value of the elements of output vector. 
	*/
	buffers(unsigned input_size, unsigned input_init, unsigned output_size, unsigned output_init) { inputBuffer.resize(input_size, input_init); outputBuffer.resize(output_size, output_init); }

	/**

	* Initializes 
	* @param <unsigned> input_size: size of the input vector.
	* @param <unsigned> input_init: value of the elements of input vector.
	* @param <unsigned> output_size: size of the output vector.
	* @param <unsigned> output_init: value of the elements of output vector.
	*/
	buffers(unsigned short * branches, int branches_size, glm::mat4 * translate, int translate_size)
	{
		for (int i = 0; i < branches_size; i++)
			inputBuffer.push_back(branches[i]); 

		for (int i = 0; i < translate_size; i++)
			translateBuffer.push_back(translate[i]); 

		finalBuffer.resize(Config::MAX_BRANCHES * 4 , glm::mat4(2.0f));//mida
	}
};

/**
* ComputeShader Class. 
* This class performs all the behaviour of a compute shader, from the creation to the dispatch. This shader only accepts: 2 buffers as input/output. @see gluintsInfo
*/
class ComputeShader {

public :
	/**
	* Basic constructor. 
	* Initialize all the variables as default constructor for each type. 
	*/
	ComputeShader();
	/**
	* Parametric constructor.
	* Create a shader from the parameter, for more information @see Init(), it also sets the debug functions enabled
	* @param <string> filepath, path to the shader source.
	* @param <bool> debug, either debug options are enabled or not. 
	*/
	ComputeShader(string filepath, bool debug = true);

	ComputeShader(string filepath, unsigned short * branches, int branches_size, glm::mat4 * translate, int translate_size,bool debug = true);
	/**
	* Initialzier function. 
	* This function generates, compiles and links the shader, and also populate the info about gluints, @see gluintsInfo
	* @param <string> filepath, path to the shader source.
	*/
	void Init(string filePath);

	/**
	* Input data handler
	* Set the input buffer from buffers struct to the data passed by this function. 
	* @param <vector <unsigned int > > data to be passed to compute buffer. 
	*/
	void SendBufferDataAsInt(vector<GLushort> data);

	/**
	* Dispatch the shaders 
	*/
	void Compute(); 

	void DebugCompute();
private: 
	gluintsInfo _information; 
	buffers _buff; 
	bool _debug; //To show or hide debugg messages.
	renderSystem rs; //Not sure if needed, just to make sure that all "enviornemnt variables" are initialized. 

	bool CompileAndLink(string filepath);//Creates the shader and program.
	bool CheckCompile();//Cheks if the last compilation was succesfull

	char *fileToChar(string filepath)const;
	
};