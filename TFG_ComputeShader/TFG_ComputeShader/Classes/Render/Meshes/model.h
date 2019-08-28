#pragma once

// Local Headers
#include "../modelLoader.h"
#include "../ShaderCompiler.h"
#include "../Renderizer.h"
#include "Mesh.h"
#include "../stb_image.h"
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
// Standard Headers
#include <iostream>
#include <string>
#include <vector>

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class model : public Renderizer {
public: 
	model(); 
	model(const model &);
	bool loadModel(string path, bool debug = false);
	bool setBuffers(); 
	void set_shaders(string vertexShader, string fragmentShader);
	unsigned int get_vShader()const { return (_meshes.size() > 0) ? _meshes[0].get_vShader() : 0; }
	unsigned int get_fShader()const { return (_meshes.size() > 0) ? _meshes[0].get_fShader() : 0; }
	unsigned int get_program()const { return (_meshes.size() > 0) ? _meshes[0].get_program() : 0; }
	void draw();
private: 
	vector<Mesh> _meshes; 
	string directory; 
	void ProcessNodeStructure(aiNode *node, const aiScene * scene);
	Mesh processMesh(aiMesh * mesh, const aiScene * scene);
	void normalizeVertexAxis(vector<Vertex> &v, float minX, float minY, float minZ, float normFactor, float,float,float, bool debug = false);

	vector<Texture>loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);



};
