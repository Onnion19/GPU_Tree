#pragma once
// Local Headers
#include "modelLoader.h"
#include "Meshes/Triangle.h"
#include "Meshes/Cube.h"
#include "Meshes/model.h"
#include "Renderizer.h"
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
// Standard Headers
#include <cstdio>
#include <cstdlib>

using namespace std;

class renderSystem {
public:
	renderSystem();
	/** Fill the transform buffer
	*	Given a Transform Matrix buffer it fills the Render Matrix for every mesh.
	*	\param transforms <vector glm::mat4> Buffer with all the transform stored
	*	\param chunks <unsigned int> How many transforms should be copied.
	*/
	void SetTransformsRenderSystem(vector<glm::mat4> transforms, unsigned int chunks);
	/** Adds a single element to the Render buffers
	*	Adds an element to be rendered at every frame 
	*	\param mesh <Renderizer> mesh to be rendered. Has to implement draw function 
	*	\param transform <glm::mat4> transform where should it be rendered. By deafualt is Identity Matrix
	*/
	void addElement(Renderizer * mesh, glm::mat4 t = glm::mat4(1.0f)); 
	/*
	* Rset the default values for the camera settings
	*/
	void ResetCamera();
	/*******************RENDER TYPES*********************/
	/** Renders a bunch of items that shares the same mesh. 
	*	This function groups a number of models into a root Transform in WorldSpace, This is perfect for tree renderer because multiple models form a tree, which have only one root. 
	*	The function allows to send data of multiple tree. 
	*	\param bufferTreeChunk <vector short> How many mesh have each tree. 
	*	\param RootTransform <vector glm::mat4> Vector with all the roots for each tree. 
	*/
	void RenderInstancingCubes(vector<short> bufferTreeChunk, vector<glm::mat4> RootTransform);
	/** Render Once
	*	Render the scene once
	*/
	void UNIQUERender();
	/** Render debug triangle
	*	Function for debug purposees, draws a triangle on the screen. 
	*	\param t <triangle *> Triangle information to be rendered. 
	*/
	void RenderTriangle(Triangle * t); 
	/** Render debug cube
	*	Function for debug purposes, draws a cube on the screen 
	*	\param c <Cube *> Cube infromation to be rendered. 
	*/
	void RenderCube(Cube * c);
	/** Render debug Model
	*	Function for debug purposes, draws a model on the screen
	*	\param m <model *> Model information to be rendered.
	*/
	void RenderModel(model *m);
	/** Render Loop
	*	Classical render loop which updates every frane. 
	*/
	void RenderLoop();

	//Debug Pruposes
	void mostrarMatriu(glm::mat4 m)const { mainCamera->mostraMatriu(m); }
private: 
	struct camera {
	public:
		camera() {
			init();
		}
		void init() {
			this->position = glm::vec3(3.0f, 3.0f, -15.0f);
			set_target(glm::vec3(0.0f, 3.0f, 0.0f));
			up = glm::vec3(0.0f, 1.0f, 0.0f);
			set_right();
			set_up();
			setw_h(1280,800);
			projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
			model = glm::mat4(1.0f);
			set_view();
			//mostrar();
			als = 3; 
			radi = 10;
			angle = 0;
		}
		void update() {
			position = glm::vec3(radi * sin(angle),als,radi*cos(angle));
			target = glm::vec3(0, als, 0);
			direction = glm::normalize(target - position); 
			set_up();
			up = glm::vec3(0, 1, 0);
			set_view();
			//mostrar();
		}

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		void UP() { /*position.y += movement;*/ als += 0.5; }
		void DOWN() { /*position.y -= movement;*/als -= 0.5;}
		void LEFT() { /*position.x -= movement;*/angle -= 0.1; }
		void RIGHT() { /*position.x += movement;*/angle += 0.1; }
		void ZOOMIN() { /*position.z += movement;*/ radi -= 1; }
		void ZOOMOUT() { /*position.z -= movement; */radi += 1; }
		void set_view() {view = glm::lookAt(position, direction, up);}
		void set_up() {	up = glm::cross(direction, right);	}
		void set_target(glm::vec3 t) {target = t; direction = glm::normalize(target - position);	}
		void set_position(glm::vec3 p) {position = p; direction = glm::normalize(target - position);	}
		void set_right() { right = glm::normalize(glm::cross(up, direction)); }
		void setw_h(unsigned int w, unsigned int h){width = w; height = h;}
		void set_direction(glm::vec3 d) { direction = d; }
		glm::mat4 get_model()const { return model;}
		glm::mat4 get_view()const { return view; }
		glm::mat4 get_projection()const { return projection; }
		glm::vec3 get_up()const { return up; }
		glm::vec3 get_pos()const { return position; }
		glm::vec3 get_target()const { return target; }
		glm::vec3 get_direction()const { return direction; }
		unsigned int get_width()const { return width; }
		unsigned int get_height()const {return height;}

		void mostrar()
		{
			cerr << "CAMERA SETTINGS: " << endl; 
			cerr << "\t POSITION: "; mostraVector(position);cerr<< endl;
			cerr << "\t TARGET: "; mostraVector(target); cerr << endl;
			cerr << "\t \t DIRECTION: "; mostraVector(direction); cerr << endl;
			cerr << "\t UP: "; mostraVector(up); cerr << endl; 
			cerr << "\t RIGHT: "; mostraVector(right); cerr << endl;
			cerr << "\t MODEL: "; cerr << endl; mostraMatriu(model);
			cerr << "\t VIEW: "; cerr << endl; mostraMatriu(view);
			cerr << "\t PROJECTION: "; cerr << endl; mostraMatriu(projection);

		}
		//DEBUG
		void mostraVector(glm::vec3 v)
		{
			cerr << v.x << " , " << v.y << " , " << v.z;
		}
		void mostraMatriu(glm::mat4 m)
		{
			for (int i = 0; i < 4; i++)
			{
				cerr << "\t    ";
				for (int j = 0; j < 4; j++)
				{
					cerr << m[j][i] << "  ";
				}
				cerr << endl;
			}
		}
	private: 
		glm::vec3 up;
		glm::vec3 right;

		glm::vec3 position;
		glm::vec3 target; //Direccio
		glm::vec3 direction;
		unsigned int width, height;
		const float movement = 0.2;
		float radi; 
		float als; 
		float angle; 
	};
	bool enviadades; 
	vector<Triangle> _Telements;
	vector<Renderizer *> _elements;
	vector<model *> _models;
	camera * mainCamera = new camera(); 
	vector<glm::mat4> _transforms;
	Assimp::Importer assimp_importer;
	GLFWwindow * mainWindow = nullptr;
	void initGLFW();
	void setDefaultCamera();
	void sendUniforms(unsigned int _IDprogram,glm::mat4 transform = glm::mat4(1.0f), glm::mat4 root = glm::mat4(1.0f));
	void cameraUP();
	void cameraInput();

	model * _tronc;
};