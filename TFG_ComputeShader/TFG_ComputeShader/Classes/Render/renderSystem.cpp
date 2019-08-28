#include "renderSystem.h"

renderSystem::renderSystem()
{
	initGLFW();
	gladLoadGL();
	enviadades = false;

	cerr << "Initating Glad and glfw" << endl;
	/*This chunk of code only check that Compute Shader is enabled on your PC-Hardware.*/
	// Finding the compute shader extension
	int extCount;
	glGetIntegerv(GL_NUM_EXTENSIONS, &extCount);
	bool found = false;
	for (int i = 0; i < extCount; ++i)
		if (!strcmp((const char*)glGetStringi(GL_EXTENSIONS, i), "GL_ARB_compute_shader")) {
			printf("Extension \"GL_ARB_compute_shader\" found\n");
			found = true;
			break;
		}

	if (!found) {
		fprintf(stderr, "Extension \"GL_ARB_compute_shader\" not found\n");
		exit(14);
	}
	
}

void renderSystem::SetTransformsRenderSystem(vector <glm::mat4 > t , unsigned int trunksCount)
{

	/*
		 DISCLAIMER: THIS PROCESS IS NOT OPTIMIZED!!!
		 For a better optimization and perfomance consider creating some instancing methods rather than just send the same geometry hundreds of times. 
		 The current solution is causing low fps rates, and poor perfomance. (but hey, it works if you only want to see the result!) 
	*/


	//Clear the current Transform Matrix buffer, and copy the new one. 
	_transforms.clear(); 
	_transforms.insert(_transforms.begin(), t.begin(), t.begin() + trunksCount);

	//Clear the tronc model if any
	if (_tronc != nullptr)
		delete _tronc;
	//Load a new model
	_tronc = new model();
	if (!_tronc->loadModel("cube.obj"))
	{
		cout << "Error loading the model" << endl;
		exit(-1);
	}

	//Get the shaders to apply to the Model 
	string vc = "TriangleVShader.glsl";
	string fc = "TriangleFShader.glsl";
	//Send shaders data and complie them
	_tronc->set_shaders(vc, fc);
	//Create vertex buffers and others. 
	_tronc->setBuffers();

	//Update _elements buffer to the current one.
	_elements.resize(trunksCount, _tronc);
}


void renderSystem::RenderInstancingCubes(vector<short> bufferTreeChunk , vector <glm::mat4>RootTransforms)
{

	/**
		DISCLAIMER: THIS PROCESS IS NOT OPTIMIZED!!!
		 For a better optimization and perfomance consider creating some instancing methods rather than just send the same geometry hundreds of times. 
		 The current solution is causing low fps rates, and poor perfomance. (but hey, it works if you only want to see the result!) 
	*/
	while (glfwWindowShouldClose(mainWindow) == false) {
		if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mainWindow, true);
		cameraInput();
		// Background Fill Color
		glClearColor(0.05f, 0.05f, 0.20f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Check Tree count
		int current_tree = 0;
		int chunks = 0;
		for (int i = 0; i < _elements.size(); i++)
		{	
			if (chunks > bufferTreeChunk[current_tree]-1) //If we have processed all items of a tree. 
			{
				current_tree++; chunks = 0;
			}
			//Send uniforms as: ProgramID, Relative Mesh Transform, Root Transform). Vertex Shader will calculate it's world space. 
			sendUniforms(_elements[i]->get_program(),_transforms[i], RootTransforms[current_tree]);
			_elements[i]->draw();
			chunks++;
			
		}
		// Flip Buffers and Draw
		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}
	glfwTerminate();
}

void renderSystem::initGLFW()
{
	// Load GLFW and Create a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	mainWindow = glfwCreateWindow(1920, 1080, "OpenGL", nullptr, nullptr);

	// Check for Valid Context
	if (mainWindow == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		throw EXIT_FAILURE;
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(mainWindow);
}




void renderSystem::addElement(Renderizer* m, glm::mat4 t)
{
	_elements.push_back(m);
	_transforms.push_back(t);
}


void renderSystem::ResetCamera()
{
	setDefaultCamera();
}


void renderSystem::RenderCube(Cube * c)
{
	while(glfwWindowShouldClose(mainWindow) == false) {
		if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mainWindow, true);
		
		cameraInput();
			 
		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//t.updateFrame();
		sendUniforms(c->get_program());
		c->draw();
		// Flip Buffers and Draw
		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
		
	}

	glfwTerminate();
}
void renderSystem::RenderTriangle( Triangle * t)
{
	while (glfwWindowShouldClose(mainWindow) == false) {
		if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mainWindow, true);

		cameraInput();
		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//t.updateFrame();
		sendUniforms(t->get_program());
		t->draw();
		// Flip Buffers and Draw
		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}
	
	glfwTerminate();
}

void renderSystem::RenderModel(model * m)
{
	while (glfwWindowShouldClose(mainWindow) == false) {
		if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mainWindow, true);

		cameraInput();

		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//t.updateFrame();
		sendUniforms(m->get_program());
		m->draw();
		// Flip Buffers and Draw
		glfwSwapBuffers(mainWindow);
		glfwPollEvents();

	}

	glfwTerminate();
}


void renderSystem::UNIQUERender()
{
	// Background Fill Color
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < _elements.size(); i++)
	{
		sendUniforms(_elements[i]->get_program(),_transforms[i]);
		_elements[i]->draw();
	}

	// Flip Buffers and Draw
	glfwSwapBuffers(mainWindow);
	glfwPollEvents();
	glfwTerminate();
}

void renderSystem::RenderLoop()
{
	while (glfwWindowShouldClose(mainWindow) == false) {
		if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mainWindow, true);
		cameraInput();
		// Background Fill Color
		glClearColor(0.05f, 0.05f, 0.20f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		for (int i = 0; i < _elements.size(); i++)
		{
			sendUniforms(_elements[i]->get_program(),_transforms[i]);//Comrpvoar que el shader no estigui sobre-escrit a la matriu identitat
			_elements[i]->draw();
		}

		// Flip Buffers and Draw
		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}
	glfwTerminate();
}

void renderSystem::sendUniforms(unsigned int _IDprogram, glm::mat4 transform, glm::mat4 root)
{	
	int viewloc, modelLoc, projloc,transloc,rootloc; 

	const GLchar * v = "view";
	const GLchar * m = "model"; 
	const GLchar * p = "projection";
	const GLchar * t = "transform";
	const GLchar * r = "root";

	//Get Uniforms ID 
	glUseProgram(_IDprogram);
	viewloc = glGetUniformLocation(_IDprogram, v);
	modelLoc = glGetUniformLocation(_IDprogram, m);
	projloc = glGetUniformLocation(_IDprogram, p);
	transloc = glGetUniformLocation(_IDprogram, t);
	rootloc = glGetUniformLocation(_IDprogram, r);

	//Send Unifroms data
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mainCamera->model));
	glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(mainCamera->view));
	glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(mainCamera->projection));
	glUniformMatrix4fv(transloc, 1, GL_FALSE, glm::value_ptr(transform));
	glUniformMatrix4fv(rootloc, 1, GL_FALSE, glm::value_ptr(root));
}

void renderSystem::setDefaultCamera()
{
	mainCamera->init();
}


void renderSystem::cameraInput()
{
	bool update = true; 
	if (glfwGetKey(mainWindow, GLFW_KEY_W) == GLFW_PRESS)
		mainCamera->UP();
	else if (glfwGetKey(mainWindow, GLFW_KEY_S) == GLFW_PRESS)
		mainCamera->DOWN();
	else if (glfwGetKey(mainWindow, GLFW_KEY_A) == GLFW_PRESS)
		mainCamera->LEFT();
	else if (glfwGetKey(mainWindow, GLFW_KEY_D) == GLFW_PRESS)
		mainCamera->RIGHT();
	else if (glfwGetKey(mainWindow, GLFW_KEY_Q) == GLFW_PRESS)
		mainCamera->ZOOMOUT();
	else if (glfwGetKey(mainWindow, GLFW_KEY_E) == GLFW_PRESS)
		mainCamera->ZOOMIN();
	else 
		update = false; 

	if (update)
		mainCamera->update();
	
}
void renderSystem::cameraUP()
{
	mainCamera->UP();
	cout << "Posicio de la camera: "; 
	mainCamera->mostraVector(mainCamera->get_pos());
	cout << endl;
}