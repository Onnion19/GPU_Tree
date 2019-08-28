#pragma once

#include "../Renderizer.h"
#include "../ShaderCompiler.h"
// System Headers
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

class Cube : public Renderizer
{
public:
	Cube();
	void set_punts(glm::vec3 A, glm::vec3 B, glm::vec3 C ,glm::vec3 D);
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
	unsigned int idVindices;
	float _points[24];
	GLubyte _indices[36];
	ShaderCompiler _sc;

	void createIndices(); 
};