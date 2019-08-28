#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Dictionary.h"
#include "StringTree.h"
#include "Config.h"
using namespace std; 
//! LSystem class, refers user-API to generate L-System tree
/*!
	LSystem class is a Singleton where the user can create trees on demand, the class will recieve a set of rules and an axiom in order to start generate. 
	Attention: It's important to use the string variables of the dictionary, otherwise the class will not work and the returning string may be incoherent.
*/
class LSystem 
{
public:

	struct generation {
		unsigned short branches;
		char buffer[Config::MAX_BRANCHES * Config::MAX_WIDTH];
	};
	//!Initializer by file
	/*!
		The user must provide a path file with the axiom and the rules. It must fit the following format: 
		AXIOM
		(Empty lane) 
		List of rules ( Character to be replaced : String to replace)
		\param filename string with relative or absolute path of the file
		\return L-System pointer to the new L-System
	*/
	static LSystem * init(const string filename); 

	//!Default Initializer
	/*!
		Creates an L-System and return the pointer to the user. If an L-System is already created, then returns it's pointer.
	*/
	static LSystem * init();

	//!Default Destructor
	/*!
		Sets the pointer to null and clear all the data on the class
	*/
	~LSystem();

	//! Adds a new rule to the dictionary
	/*!
		\param item <char> string to be replaced
		\param replace <string> string to be placed
		\return bool If the rules is added or not
	*/
	bool AddRule(char item, string replace); 

	//! Changes axiom
	/*!
		Changes the actual Axiom for the new One. If the string is empty, no change is applied!
		\param axiom <string> The new axiom to be placed
	*/
	void NewAxiom(string axiom);
	//!DefaultTreeGeneration
	/*!
		It generates the tree with the actual parameters. An Axiom, rules and generations must be set before in order to generate a tree;
		\param int number of generations.
		\param bool debug If want to debugg the tree generation process via screen, False by default.
		\return string Returns the final tree. If it can not be generate that the string should be empty.
	*/
	string GenerateTree(int generations = 1,bool debug = false);

	//!TreeGeneration for GPU
	/*!
	It generates the tree with the actual parameters. An Axiom, rules and generations must be set before in order to generate a tree;
	\param char * Buffer to store all the generation of the tree
	\param unsigned int * indiceBuffer  to store the indices where a generations starts in the buffer position 
	\param int number of generations.
	\param bool debug If want to debugg the tree generation process via screen, False by default.
	\return string Returns the final tree. If it can not be generate that the string should be empty.
	*/
	void GenerateTree(unsigned short * buffer, unsigned short * indiceBuffer, unsigned int generations = 1, bool debug = false);

	int GenerateTreesFromFile(unsigned short * buffer, unsigned int * indiceBuffer, string filename, unsigned int generations = 1, bool debug = false);
	//!Generates a tree and output to a file
	/*!
		It generates the tree with the actual parameters. An Axiom, rules and generations must be set before in order to generate a tree;
		\param int number of generations.
		\param string pathfile The path of the output file. If it does not exist, it will create it
		\param bool debug If want to debugg the tree generation process via screen, False by default.
		\return vector<string> returns a vector with the trees. If it can not be generate that the string should be empty. 
	*/
	vector<string> GenerateTreeFromFile(string pathfile, int generations = 1,bool debug = false);
	
	vector<unsigned short> TranslateCurrentTree(bool debug = false);
	void showItems()const;


private: 
	LSystem();
	static LSystem * _lSystem;
	string _Axiom; //IS THIS NECESSARY?
	string _Tree;  // IS THIS NECESSARY? 
	Dictionary _Dictionary; 
	StringTree _StringTree;

	//DATA STRUCTURE FOR SAVE MULTIPLE GENERATIONS 




	//Functions
	bool ReadFile(const string filename);
	void Clear();
};

