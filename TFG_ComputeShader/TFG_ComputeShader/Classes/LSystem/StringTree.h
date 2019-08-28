#pragma once

#include <iostream>;
#include <fstream>;
#include <vector>;
#include "Dictionary.h"; 
#include "Config.h";

class StringTree
{
public:
	StringTree();

	//!Generates a tree
	/*!
		Given an Axiom, a Dictionary and the number of generations it, creates  the resulting tree in L-System
		\param Axiom <string> the root of the tree
		\param Dic <Dictionary> the Dictionary where to find the symbols and rules
		\param generations <int> (Optional), the iterations of the algorism, 1 By default
		\param Debug <bool> (Optiona), see the progress on the screen, False by default.
		\return string the resulting tree, if it faisl generating, an empty string is returned
	*/
	string Generate(string Axiom, Dictionary Dic, int generations = 1, bool Debug = false );

	//!Generates " n " from file
	/*!
		Given a file full of Axioms, it generate all the trees from those Axioms. (Each Axiom must be in a new line); 
		\param path <string> the path to the file
		\param Dic <Dictionary> the Dictionary where to find the symbols and rules
		\param generations <int> (Optional), the iterations of the algorism, 1 By default
		\param Debug <bool> (Optiona), see the progress on the screen, False by default.
		\return vector<string> the resulting trees, if it fails generating, an empty string is returned
	*/
	vector <string> GenerateFromFile(string pathFile, Dictionary dic, int generations = 1, bool Debug = false);

	//! Translate current tree from characters to numbers
	/*!
	From the current tree string, it is translated using a dictionary to a numbers. The numbers goes from 0 to n. For more details see the dictionary class reference. 
	\param dic <Dictionary> the Dictionary where to find the symbols. 
	*/
	vector<unsigned short> TranslateCurrentTree(Dictionary dic, bool debug = false);

	//!Generates a tree to a buffer
	/*!
		Given an initial Axiom and the number of generations, it creates a tree in GPU-FORM to a buffer. In the buffer is stored every generation created until reach the final one.
		\param buffer <unsigned short *> buffer where the tree will be stored 
		\param indiceBuffer <unsigned short *> buffer to store where one generations starts and ends. Each position means the ending of that generation index(ie: [0] = ending pos for generation 0)
		\param Axiom <string> Initial string to start generating the tree. 
		\param dic <Dictionary> Which rules must be aplied. 
		\param generations <unsigned int> Number of iterations in the L-System process. 
		\param debug <bool> Enable or disable debug messages. 
	*/
	void GenerateToBuffer(unsigned short * buffer, unsigned short * indiceBuffer, string Axiom, Dictionary dic, unsigned int generations = 1, bool debug = false);
	//! Generates multiple tree to a buffer
	/*!  Given a file with all axioms to be processed, it process them one by one with the same algorithm as @see GenerateToBuffer. The main difference is that for store management and optimize the process 
	* only the last generation is saved for each tree. Eventhought we need to caclulate all the generations in order to reach the last one. 
	*	\param buffer <unsigned short *> buffer where the last generation of all trees are stored. 
	*	\param indiceTreeBuffer <unsigned int *> buffer to store where each tree starts in the buffer. 
	*	\param filename <string> Name of the file to retrieve the axioms to process.
	*	\param dic <Dictionary> Rules to apply when generate the L-System
	*	\param generations <unsigned int> Number of generations processed for every tree. (Only the last is saved) 
	*	\param debug <bool> Should display debug messages? 
	*	\return <int> number of tree generated. 
	*/
	int GenerateMultipleToBuffer(unsigned short * buffer, unsigned int * indiceTreeBuffer, string filename, Dictionary dic, unsigned int generations = 1, bool debug = false);
private: 

	string _axiom; 
	string _tree; 
	unsigned int ProcessAxiom(unsigned short * buffer, unsigned int initalPositionWrite, unsigned short initalDepth, string Axiom, Dictionary _dic);
	bool GenerateNewBranch(unsigned short * buffer, unsigned short * branches, unsigned int * currentWritablePos, unsigned short * currentDepth, unsigned int initalPositionWrite);
	unsigned int ProcessNewGeneration(unsigned short * buffer, unsigned short * indiceBuffer,unsigned int currentGeneration, Dictionary _dic, bool debug = false);

	//Start reading at the begin of the branch (depth level).
	unsigned int ProcessBranch(unsigned short * buffer, unsigned int readingBufferPos, unsigned int writtingBufferPos, Dictionary dic, bool debug);
	bool BranchItemValid(char itemToEvaluate)const;
	vector<string> ReadAxiomsFromFile(string filename)const; 
};

