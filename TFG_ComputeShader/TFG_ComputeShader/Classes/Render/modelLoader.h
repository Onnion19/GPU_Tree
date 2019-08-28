#pragma once

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// Standard Headers
#include <iostream>
#include <string>


//This class is a template from Assimp Tutorials, more info at https://learnopengl.com/Model-Loading/Assimp

class modelLoader
{
public:
		modelLoader();
		modelLoader(const std::string filepath);
		const aiScene * loadObject(const std::string file, bool debug = false);
		std::string getError()const;
		~modelLoader();
private: 
	const aiScene * scene; 
	Assimp::Importer importer; 
};

