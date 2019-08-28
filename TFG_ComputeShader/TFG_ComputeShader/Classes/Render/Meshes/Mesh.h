#pragma once 



// Local Headers
#include "../modelLoader.h"
#include "../ShaderCompiler.h"
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
#include <glm/glm.hpp>
using namespace std;

/**
* Struct for vertex data.
* Stores the main information about vertex data, such as position, normal direction and Texture Coordinates.
*/
struct Vertex {
	glm::vec3 _posicio;
	glm::vec3 _normal;
	glm::vec2 _texCoords;
};

/**
* Struct for Texture data
* Stores the main information about Textures, such as number id, type of the texture (Diffuse, Normal,..) and the path where the textures is located.
*/
struct Texture {
	unsigned int id;
	string type;
	aiString path;
};

//This class was created following learnopengl tutorials. All documentation can be found at: https://learnopengl.com/Model-Loading/Mesh
class Mesh {
	
public: 
	Mesh(const Mesh &m);
	Mesh(vector<Vertex> vertex, vector<unsigned int> indices, vector<Texture> textures); 
	//Not use LoadMesh. 
	bool loadMesh(string path, bool debug = false);
	bool setBuffers();
	void draw();
	void set_shaders(string vertexShader, string fragmentShader);
	unsigned int get_vShader()const { return _sc.get_vShader(); }
	unsigned int get_fShader()const { return _sc.get_fShader(); }
	unsigned int get_program()const { return _sc.getProgram(); }
	string get_vShaderName()const { return _sc.get_vShaderName(); }

private: 
	Mesh();
	vector<Vertex> _vertices; 
	vector<unsigned int> _indices; 
	vector<Texture> _textures;
	ShaderCompiler _sc;
	modelLoader _ml;

	unsigned int _IDVao, _IDVbo, _IDVeo; 
};