#include "renderSystem.h"

renderSystem::renderSystem()
{
	initGLFW();
	gladLoadGL();
	enviadades = false;


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



void renderSystem::initGLFW()
{
	// Load GLFW and Create a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	mainWindow = glfwCreateWindow(800, 800, "OpenGL", nullptr, nullptr);

	// Check for Valid Context
	if (mainWindow == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		throw EXIT_FAILURE;
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(mainWindow);
}



void renderSystem::AddTriangle(Triangle t)
{
	//_elements.push_back(t);
}
void renderSystem::addElement(Renderizer* m, glm::mat4 t)
{
	_elements.push_back(m);
	_transforms.push_back(t);
}

void renderSystem::setCamera(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	//mainCamera->set
	//mainCamera->view = view;
	//mainCamera->projection = projection;
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

void renderSystem::sendUniforms(unsigned int _IDprogram,glm::mat4 transform)
{	
	int viewloc, modelLoc, projloc,transloc; //Int perque retorna -1 en cas de no trobar

	const GLchar * v = "view";
	const GLchar * m = "model"; 
	const GLchar * p = "projection";
	const GLchar * t = "transform";

	glUseProgram(_IDprogram);//Nomes es necessari si hi ha diferents shaders funcionant.
	viewloc = glGetUniformLocation(_IDprogram, v);
	modelLoc = glGetUniformLocation(_IDprogram, m);
	projloc = glGetUniformLocation(_IDprogram, p);
	transloc = glGetUniformLocation(_IDprogram, t);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mainCamera->model));
	glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(mainCamera->view));
	glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(mainCamera->projection));
	glUniformMatrix4fv(transloc, 1, GL_FALSE, glm::value_ptr(transform));
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