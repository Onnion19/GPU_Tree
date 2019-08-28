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
	/*Prepare all the buffers for the GPU such as VertexBuffer*/
	virtual bool setBuffers() = 0;
	/*Compiles the shaders pointed by their filepath*/
	virtual void set_shaders(string vertexShader, string fragmentShader) = 0;
	/*Get vertex Shader GPU ID */
	virtual unsigned int get_vShader()const = 0;
	/*Get fragment Shader GPU ID */
	virtual unsigned int get_fShader()const = 0;
	/*Get compiled program GPU ID */
	virtual unsigned int get_program()const = 0;
	/*Draw function to be displayed at screen*/
	virtual void draw() = 0;
};