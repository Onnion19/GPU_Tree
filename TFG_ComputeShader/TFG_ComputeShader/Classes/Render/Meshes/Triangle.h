#pragma once



#include "../ShaderCompiler.h"
#include "../Renderizer.h"
// System Headers
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

class Triangle : public Renderizer
{
public: 
	Triangle();
	void set_punts(glm::vec3 A, glm::vec3 B, glm::vec3 C);
	bool setBuffers(); 
	void set_shaders(string vertexShader, string fragmentShader);
	unsigned int get_vShader()const; 
	unsigned int get_fShader()const; 
	unsigned int get_program()const { return _sc.getProgram(); }
	void updateFrame();
	void draw();
private: 
	unsigned int idVBO; 
	unsigned int idVAO;
	float _points[9];
	ShaderCompiler _sc; 
	vector<glm::vec3> _punts;

};