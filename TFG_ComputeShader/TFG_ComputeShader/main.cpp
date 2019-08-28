/**
	Developed by Aleix Rius 
	Starting Date: 12 - 02 - 19
	L-System inside GPU 
	License: GPL v3 (credit is welcome) 
*/

/*
	This project requires the references of glad, assimp, glfw, glm

	glad:	https://glad.dav1d.de/
	assimp: http://www.assimp.org/
	glfw:	https://www.glfw.org/
	glm:	https://glm.g-truc.net/0.9.9/index.html

	Thanks that all opensource code that made my work possible. 
*/
#include <iostream>

#include "Classes/Render/ComputeShader.h"
#include "Classes/Render/TreeRenderer.h" //Only for manual Tree
#include "Classes/LSystem/LSystem.h"
#include "Classes/LSystem/Config.h"
#include "Classes/LSystem/LSystemMath.h"


// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
// Standard Headers
#include <cstdio>
#include <cstdlib>

/****
	This project is an innovative way to process L-System inside the GPU. There isn't much information so this is my personal approach, it may be wrong. 
	Furthermore, this project is not focused in optimize rendering algorythms but optimize multiple tree generation using L-System, so the main effort is at L-System data 
	managment and performance. 
	
	Render Algorithm are not optimized as well as, the rendering has the only purpose to visualize the result. 

*/

/***
	This program has 3 phases: 
	
	- Data input: Which is mostly done by txt file located at the same directory as the executable. This phase will load: 
		· L-System dictionary and rules. (rules.txt / simbols.txt) 
		· L-System axioms for every Tree. (axioms.txt). 
		· Load models to be renderized @see Mesh.h for more information. 
		· Load Shaders (compute, vertex and fragment9. 
	
	
	- L-System Pre-Process: This phase read the axiom and process the generation in a  specifid buffer format optimized for GPU. This process uses the CPU (Still looking how calculate on GPU): 
		· For this buffer format we have to consider a couple of things. 
			# Every time a branch splits, both trunks will form a new branch. In other words, a branch is valid until it splits, then 2 new branch are created.
			# Every branch is formed by a group of meshes, in this case, cubes. Usuall called chunks. 
			# Buffer have to be imagine as an Array, where each row is a branch. Sometimes all the row will be filled with data, other times, only some spaces will. 


	- GPU L-System Process: This phase take all the L-System algebra and transform to Matrix corresponding to tree hierachy. And allowing multiple tree calculations despite of the data is stored 
		in the same buffer. 
			· This phase is made of 2 compue shaders: 
				# First of all transforms all the L-System algebra to Matrix. And calculates the trasnform being relative to it's branch. (Branch space). 
				# Second Determine's the branch owner (Which tree has that branch) and calculates all the chunks Transform to Tree-Space. 
			· Each compute shader unit process a branch, despite of, which tree owns that branch. 

	The final result is a buffer with a lot of Transforms in Tree-Space with the root at (0,0,0) (Yes I know, If All tree's have the root at the same place, it will be a mess). 
	Vertex Shader will be the one who calculates each transform to World Space, giving each tree a Root Transform in worldSpace coordinates. 


*/


using namespace std;

string CrearString();


int main() {

	cout << "How many generations do you want to create (Recomended 2 - 4): " << endl;
	int generations;
	cin >> generations;

	// Create Buffers for hold tree data structure
	unsigned short * buffer = new unsigned short[Config::MAX_BUFFER_SIZE]; //Holds all the L-System data structured in rows of size determined at configClass @see Config.h
	unsigned int * indices = new unsigned int[Config::MAX_TREE];//Stores indices of the starting point for each tree inside the buffer

	//Initialize, all value to 0
	for (int i = 0; i < Config::MAX_BRANCHES * Config::MAX_WIDTH * Config::MAX_GENERATIONS; i++)
	{
		buffer[i] = 0; 
		if (i < Config::MAX_GENERATIONS)
			indices[i] = 0;
	}


	/*************file tree creation******************************/
	LSystem * pointer = LSystem::init("Rules.txt");
	pointer->showItems();//Debug, display the Symbols and rules

	int numTree = pointer->GenerateTreesFromFile(buffer, indices, "axioms.txt", generations, false); //Read the axioms from file "axioms.txt" and start processing them 
	if (numTree < 0)
		throw ("Error generating trees");



	/****************Starts GPU Processing********************/

	/*****Translate the L-System data to Matrix*******/
	//Creating Buffer for store the matrix
	LSystemMath ls;
	glm::mat4 * buffer_dic = new glm::mat4[40]; // Create buffer for matrix (Size have to be automaded, check lSystemMath)

	Dictionary dic;
	ls.GenerateMatrixOperationBuffer(buffer_dic, dic, false);//Fill the buffer whith the corresponding matrix
	string path = "BranchTransformShader.glsl";

	int start = 0;
	int end = indices[numTree - 1];

	ComputeShader cs(path, numTree, false);
	cs.SetForBranchCompute(buffer, start, end, buffer_dic, 40, false);
	cs.Compute();

	//Don't finish execution, so there is time to check debug messages.
	string EnterAnyLetterToFinishExecution;
	cin >> EnterAnyLetterToFinishExecution;

	return 0; 



	/**************************** DEBUG MODE HAND TREE CREATION ************************/
	/*LSystem * pointer = LSystem::init("Rules.txt");
	string axiom;
	cin >> axiom;
	cout << "Initial tree: " << axiom << endl;
	pointer->NewAxiom(axiom);
	cout << endl << endl;
	pointer->GenerateTree(buffer, indices, generations, true);

	LSystemMath ls;
	glm::mat4 * buffer_dic = new glm::mat4[40]; // Size have to be automaded, check lSystemMath
	Dictionary dic; 
	ls.GenerateMatrixOperationBuffer(buffer_dic, dic, false);
	string path = "ComputeShaderTest.glsl";
	cout << "Branch buffer size= " << indices[generations-1] << endl;

	int start = (generations < 2) ? 0 : indices[generations - 2];
	int end = indices[generations - 1];

	ComputeShader cs(path, true);
	cs.SetForBranchCompute(buffer, start, end, buffer_dic, 40, true);
	cs.ComputeBranch();
	
	//vector< glm::mat4 > transforms = cs.GetResult();*/

	


	//TreeRenderer tr("FF[ZFZFZFZFZFZFZFZFZFZFZFZF][zFzFzFzFzFzFzFzFzFz][XFXFXFXFXFXFXFXF][xFxFxFxFxF]"); 
	//TreeRenderer tr("FFFFF[XFXF[ZFyFyZF]XF[ZFZFZFZF]zfzXfzXf]");
	//TreeRenderer tr("FFF[ZFZFF[zzfxfxf[Zf][F][XXF]FFFF]][zfzff[ZZfXfXf[zf][F][xxf]FFFF]]FFFFFF");	
	//string tree = CrearString();
	//TreeRenderer tr(tree);
	//TreeRenderer tr(tree);
	//tr.render();

	//cerr << "Testing compute shader class" << endl; 



	//return 0;
}


string CrearString() {
	LSystem * pointer = LSystem::init("Rules.txt");
	pointer->showItems();
	string axiom;
	cin >> axiom;
	cout << "Initial tree: " << axiom << endl;
	pointer->NewAxiom(axiom);
	string tree = pointer->GenerateTree(1, false);

	cout << "Final Tree: " << tree << endl;
	
	vector<unsigned short> translate = pointer->TranslateCurrentTree(true);
	cout << "Translation: " << endl;
	for (int i = 0; i < translate.size(); i++)
		cout << translate[i]<<" ";
	cout << endl;

	return tree;
}



