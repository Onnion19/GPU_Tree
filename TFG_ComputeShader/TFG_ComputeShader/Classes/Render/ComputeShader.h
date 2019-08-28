#include <iostream>

#include "../LSystem/Config.h"
#include "renderSystem.h"
#include <cstdlib>
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
	GLuint inputBuffer;//Input BufferS
	GLuint translateBuffer;//Branches buffer
	GLuint parentsBufferIndices; // Indices for hieraical tree
	GLuint computedBuffer; //1s Output Buffer
	GLuint compactComputedBuffer; //2n Output Buffer;
	gluintsInfo() {
		shaderID = programID = inputBuffer = translateBuffer = computedBuffer = compactComputedBuffer = parentsBufferIndices = 0;
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
	vector < glm::mat4 > computedBuffer;
	vector < glm::mat4 > compactComputedBuffer;
	vector < short > parentBufferIndices;
	vector < short > treeChunksCount;
	unsigned short num_branches; 
	const unsigned int max_items_branch = 16;
	buffers()
	{

	}

	buffers& operator=(const buffers& other) // copy assignment
	{
		inputBuffer = other.inputBuffer; 
		translateBuffer = other.translateBuffer; 
		computedBuffer = other.computedBuffer; 
		compactComputedBuffer = other.compactComputedBuffer;
		num_branches = other.num_branches; 
		parentBufferIndices = other.parentBufferIndices;

		return *this;
	}
	/**
	* Initializes 
	* @param <unsigned> input_size: size of the input vector.
	* @param <unsigned> input_init: value of the elements of input vector.
	* @param <unsigned> output_size: size of the output vector.
	* @param <unsigned> output_init: value of the elements of output vector.
	*/
	buffers(unsigned short * branches, unsigned int branches_start, unsigned int branches_end, glm::mat4 * translate, unsigned int translate_size)
	{
		
		for (unsigned int i = branches_start; i < branches_end; i++)
			inputBuffer.push_back(branches[i]);


		for (unsigned int i = 0; i < translate_size; i++)
			translateBuffer.push_back(translate[i]); 

		num_branches = unsigned short(inputBuffer.size() / Config::MAX_WIDTH);

		computedBuffer.resize(num_branches * max_items_branch,glm::mat4());
		compactComputedBuffer.resize(num_branches * max_items_branch - 2,glm::mat4());
		parentBufferIndices.resize(num_branches, -1);

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
	ComputeShader(string filepath, unsigned int num_trees = 0, bool debug = true);

	ComputeShader(string filepath, unsigned short * branches, int branches_start, int branches_end, glm::mat4 * translate, int translate_size,bool debug = true);
	/**
	* Initialzier function. 
	* This function generates, compiles and links the shader, and also populate the info about gluints, @see gluintsInfo
	* @param <string> filepath, path to the shader source.
	*/
	void Init(string filePath);

	/**
	* Prepare the shader to compute the branches transforms relative to it's parent. 
	* This functions receives tree data as a numeric L-System like and calculates the transforms of all the parts of the tree that has to be rendered. 
	* @param < short * > Buffer with the L-System data. 
	* @param < unsigned int> From where to start to read the buffer. 
	* @param < unsigned int > Where to stop to read the buffer (end not included). 
	* @param < glm::mat4 * > An array with the map of the L-System meanings. Symbol --> transformation matrix
	* @param < unsigned int > Size of the Array. 
	* @param < bool > Do you want debug message? 
	*/
	void SetForBranchCompute(unsigned short * branches, unsigned int branches_start, unsigned int branches_end, glm::mat4 * translate, unsigned int translate_size, bool debug = true);
	/**
	* Input data handler
	* Set the input buffer from buffers struct to the data passed by this function. 
	* @param <vector <unsigned int > > data to be passed to compute buffer. 
	*/
	void SendBufferDataAsInt(vector<GLushort> data);
	
	/** Dispatch the computes shaders and starts the render process. 
	*	WARNING: Buffer must been set before start computing, otherwise there can be an unknown behaviour. 
	*/
	void Compute();

	vector<glm::mat4> GetResult() const; 
private: 

	gluintsInfo _information; 
	buffers _buff; 
	unsigned int _treeCount;
	bool _debug; //To show or hide debugg messages.
	renderSystem rs; //Not sure if needed, just to make sure that all "enviornemnt variables" are initialized. 


	bool CompileAndLink(string filepath);//Creates the shader and program.
	bool CheckCompile();//Cheks if the last compilation was succesfull
	char *fileToChar(string filepath)const;
	//Compute branch functions
	void BufferSetUpBrancheCompute();

	/*
		Retorna una Vector amb els index de les branques pares per cada branca: 
		F[ F [ F [F [ F ] ] ] F [ F ] ] inici

		1 F 
		2 F 
		3 F 
		4 F
		5 F 
		3 F 
		4 F 

		 0	 1	 2	 3	 4	 5	 6
		[-1| 0 | 1 | 2 | 3 | 1 | 5 ]
	*/
	void RetrieveParents();
	void RetrieveParentsAndTrees();
	void SetUpRecomputeAndCompactBranches();
	void RecomputeAndCompactBranches();

};