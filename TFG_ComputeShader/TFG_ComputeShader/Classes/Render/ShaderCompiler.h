#pragma once

#include <iostream>

// System Headers
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
using namespace std; 
class ShaderCompiler {
public:
	ShaderCompiler();
	ShaderCompiler(string vShader, string fShader); 
	void set_shaders(string vShader, string fShader); //Posa els filepath dels diferents shader programs. 
	bool compile_shaders(); //Fa tot el proces de compilacio dels shaders.
	unsigned int getProgram()const;//Retorna el id del programa. 
	void clearShader();
	unsigned int get_fShader()const { return _IDfShader; }
	unsigned int get_vShader()const { return _IDvShader; }
	string get_vShaderName()const { return _vertexShader; }
private: 
	string _vertexShader; 
	string _fragmentShader; 
	bool _ok;
	unsigned int _IDvShader; 
	unsigned int _IDfShader;
	unsigned int _IDProgram;

	void init();
	char * fileToChar(string filepath)const;
	bool checkCompile(unsigned int id);
	bool checkLinking(unsigned int id);
	
};
