#include <iostream>

#include "Classes/Render/ComputeShader.h"
#include "Classes/Render/modelLoader.h"
#include "Classes/Render/Meshes/Triangle.h"
#include "Classes/Render/renderSystem.h"
#include "Classes/Render/Meshes/Cube.h"
#include "Classes/Render/Meshes/model.h"
#include "Classes/Render/TreeRenderer.h"
#include "Classes/LSystem/LSystem.h"
#include "Classes/LSystem/Config.h"
#include "Classes/LSystem/LSystemMath.h"
// System Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
// Standard Headers
#include <cstdio>
#include <cstdlib>

using namespace std;

string CrearString();
int main() {




	unsigned short * buffer = new unsigned short[Config::MAX_BRANCHES * Config::MAX_WIDTH * Config::MAX_GENERATIONS]; // Les generacions s'hauria de limitar a les que l'usuari entri
	unsigned short * indices = new unsigned short[Config::MAX_GENERATIONS];

	for (int i = 0; i < Config::MAX_BRANCHES * Config::MAX_WIDTH * Config::MAX_GENERATIONS; i++)
	{
		buffer[i] = 0; 
		if (i < Config::MAX_GENERATIONS)
			indices[i] = 0;
	}
	int generations = 3; 
	LSystem * pointer = LSystem::init("Rules.txt");
	pointer->showItems();
	string axiom;
	cin >> axiom;
	cout << "Initial tree: " << axiom << endl;
	pointer->NewAxiom(axiom);
	cout << endl << endl;
	pointer->GenerateTree(buffer, indices, generations, true);

	LSystemMath ls;
	glm::mat4 * buffer_dic = new glm::mat4[30];
	Dictionary dic; 
	ls.GenerateMatrixOperationBuffer(buffer_dic, dic, false);
	string path = "ComputeShaderTest.glsl";
	cout << "Branch buffer size= " << indices[generations-1] << endl;

	ComputeShader cs(path, buffer, int(indices[generations-1]), buffer_dic, 30, true);
	cs.Compute();




	//TreeRenderer tr("FF[ZFZFZFZFZFZFZFZFZFZFZFZF][zfzfzfzfzfzfzfzfzfz][XFXFXFXFXFXFXFXF]xfxfxfxfxf"); 
	//TreeRenderer tr("FFFFF[XFXF[ZFyFyZF]XF[ZFZFZFZF]zfzXfzXf]");
	//TreeRenderer tr("FFF[ZFZFF[zzfxfxf[Zf][F][XXF]FFFF]][zfzff[ZZfXfXf[zf][F][xxf]FFFF]]FFFFFF");	
	//string tree = CrearString();
	//TreeRenderer tr(tree);
	//TreeRenderer tr(tree);
	//tr.render();

	//cerr << "Testing compute shader class" << endl; 

/*	ComputeShader ComputeS("ComputeShaderTest.glsl"); 
	ComputeS.Compute();*/

	string a; 
	cin >> a;

	return 0;
}

string CrearString() {
	LSystem * pointer = LSystem::init("Rules.txt");
	pointer->showItems();
	string axiom;
	cin >> axiom;
	cout << "Initial tree: " << axiom << endl;
	pointer->NewAxiom(axiom);
	string tree = pointer->GenerateTree(1, false);

	cout << "Final Tree: " << tree << endl;
	
	vector<unsigned short> translate = pointer->TranslateCurrentTree(true);
	cout << "Translation: " << endl;
	for (int i = 0; i < translate.size(); i++)
		cout << translate[i]<<" ";
	cout << endl;

	return tree;
}



