#include "TreeRenderer.h"

TreeRenderer::TreeRenderer()
{
	_tree = "f";
	init();
}

TreeRenderer::TreeRenderer(string tree)
{
	_tree = tree; 
	init();
}


void TreeRenderer::init()
{
	initCube();
	initFlower();
	initModel();
	//_render.addElement(_cube);
	_render.addElement(_tronc);
}

void TreeRenderer::render()
{
	calculateTransforms(); 
	_render.RenderLoop();
}

void TreeRenderer::calculateTransforms()
{
 
	for (int i = 1; i < _tree.length(); i++)//Reccorem tota la string (el primer element no cal, perque tindrà la matriu identitat i ha de ser forward 
	{
		if (_tree[i] == 'f' || _tree[i] =='F')//forward
		{
			_matrixUtils.forward();
			_render.addElement(_tronc, _matrixUtils.get_currentMatrix());
		}
		else if (_tree[i] == '[')
		{
			_matrixUtils.queue();
		}			
		else if (_tree[i] == ']')
		{
			_matrixUtils.forward();//AFEGIM FLORETA
			_render.addElement(_flower, _matrixUtils.get_currentMatrix());
			_matrixUtils.dequeue();
		}			
		else if (_tree[i] == 'X')
		{
			_matrixUtils.rotate_x_positive();
		}
		else if (_tree[i] == 'x')
		{
			_matrixUtils.rotate_x_negative();
		}
		else if (_tree[i] == 'Y')
		{
			_matrixUtils.rotate_y_positive();
		}
		else if (_tree[i] == 'y')
		{
			_matrixUtils.rotate_y_negative();
		}
		else if (_tree[i] == 'Z')
		{
			_matrixUtils.rotate_z_positive();
		}
		else if (_tree[i] == 'z')
		{
			_matrixUtils.rotate_z_negative();
		}

		_matrixUtils.scale();
	}
	//Afegim la "floreta" a la branca principal al final de tot
	_matrixUtils.forward();
	_render.addElement(_flower, _matrixUtils.get_currentMatrix());
}

void TreeRenderer::initCube()
{
	if (_cube != nullptr)
		delete _cube; 

	_cube = new Cube(); 

	_cube->set_punts(glm::vec3(-0.1f, -0.30f, 0.0f), glm::vec3(-0.1f, 0.3f, 0.0f), glm::vec3(0.1f, 0.3f, 0.0f), glm::vec3(0.1f, -0.30f, 0.0f));
	string vc = "TriangleVShader.glsl";
	string fc = "TriangleFShader.glsl";
	_cube->set_shaders(vc, fc);
	_cube->setBuffers();

 }

void TreeRenderer::initFlower()
{
	if (_flower != nullptr)
		delete _flower;

	_flower = new Cube();

	_flower->set_punts(glm::vec3(-0.1f, -0.30f, 0.0f), glm::vec3(-0.1f, 0.3f, 0.0f), glm::vec3(0.1f, 0.3f, 0.0f), glm::vec3(0.1f, -0.30f, 0.0f));
	string vc = "TriangleVShader.glsl";
	string fc = "FlowerFShader.glsl";
	_flower->set_shaders(vc, fc);
	_flower->setBuffers();
}

void TreeRenderer::initModel()
{
	if (_tronc != nullptr)
		delete _tronc; 

	_tronc = new model();

	if (!_tronc->loadModel("cube.obj"))
	{
		cout << "Error en carregar el model" << endl; 
		exit(-1);
	}
		

	string vc = "TriangleVShader.glsl";
	string fc = "TriangleFShader.glsl";
	_tronc->set_shaders(vc, fc);
	_tronc->setBuffers();
}