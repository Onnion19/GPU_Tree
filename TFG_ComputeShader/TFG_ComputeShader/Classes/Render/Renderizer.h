#pragma once

// Local Headers
#include "modelLoader.h"
#include "ShaderCompiler.h"
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// Standard Headers
#include <iostream>
#include <string>
#include <vector>

using namespace std;



class Renderizer {
public: 
	 virtual bool setBuffers() = 0;
	 virtual void set_shaders(string vertexShader, string fragmentShader) = 0;
	 virtual unsigned int get_vShader()const = 0;
	 virtual unsigned int get_fShader()const = 0;
	 virtual unsigned int get_program()const = 0;
	virtual void draw() = 0;

private: 


};