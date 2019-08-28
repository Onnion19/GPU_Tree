#include "Cube.h"

Cube::Cube()
{

}

void Cube::set_punts(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D)
{
	_points[0] = A.r;
	_points[1] = A.g;
	_points[2] = A.b;

	_points[3] = B.r;
	_points[4] = B.g;
	_points[5] = B.b;

	_points[6] = C.r;
	_points[7] = C.g;
	_points[8] = C.b;

	_points[9] = D.r;
	_points[10] = D.g;
	_points[11] = D.b;

	int i = 12; 
	_points[i] = _points[i-12]; i++;
	_points[i] = _points[i-12]; i++;
	_points[i] = _points[i-12] + 0.2f; i++;//i = 14 - 12 = 2

	_points[i] = _points[i - 12]; i++;
	_points[i] = _points[i - 12]; i++;
	_points[i] = _points[i - 12] + 0.2f; i++;

	_points[i] = _points[i - 12]; i++;
	_points[i] = _points[i - 12]; i++;
	_points[i] = _points[i - 12] + 0.2f; i++;

	_points[i] = _points[i - 12]; i++;
	_points[i] = _points[i - 12]; i++;
	_points[i] = _points[i - 12] + 0.2f; i++;//i = 23 - 12 = 11

	createIndices();
	
}

bool Cube::setBuffers()//Hauria de ser una altre classe
{
	//Creem el VAO	
	glGenVertexArrays(1, &idVAO);//Generem Vao
	glGenBuffers(1, &idVBO);//Generemo VBO
	glGenBuffers(1, &idVindices);//Generem el buffer de index

	glBindVertexArray(idVAO);//Bind del vertex Array
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, idVBO);//Bind del vertx object
	glBufferData(GL_ARRAY_BUFFER, sizeof(_points), _points, GL_STATIC_DRAW);//Enviem les dades
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//Expliquem com agafar les dades

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idVindices);//Fem el bind 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);//Enviem les dades

	if (idVAO == 0 || idVBO == 0 || idVindices == 0)
	{
		cerr << "ALGUNA COSA HA FALLAT" << endl;
	}
	else
		cout << "TOT HA ANAT Be" << endl;

	GLenum error = glGetError();
	while (error != GL_NO_ERROR)
	{
		cerr << "Error en generar els buffers: " << error << endl;
		error = glGetError();
	}
	return true;
}

void Cube::set_shaders(string vshader, string fshader)
{
	_sc.set_shaders(vshader, fshader);
}


unsigned int Cube::get_fShader()const
{
	return _sc.get_fShader();
}

unsigned int Cube::get_vShader()const
{
	return _sc.get_vShader();
}


void Cube::updateFrame() {
	_sc.compile_shaders();//No tinc clar que faci falta, ja que el programa dins de la gpu no s'esborra fins que acaba l'app
}

void Cube::draw()
{
	glBindVertexArray(idVAO);
	glDrawElements(GL_TRIANGLES, sizeof(_indices), GL_UNSIGNED_BYTE, (void*)0);
}


void Cube::createIndices()
{
	int i = 0;
	//1 - 4 -2 / 2 - 4 - 3 (cara frontal) 
	/*
			2			3
			

			1			4
	
	*/
	_indices[i] = 0; i++;
	_indices[i] = 3; i++;
	_indices[i] = 1; i++;

	_indices[i] = 1; i++;
	_indices[i] = 3; i++;
	_indices[i] = 2; i++;
	//4-8-3 / 3-8-7 (cara lateral) 
	/*
			3			7


			4			8

	*/
	_indices[i] = 3; i++;
	_indices[i] = 7; i++;
	_indices[i] = 2; i++;

	_indices[i] = 2; i++;
	_indices[i] = 7; i++;
	_indices[i] = 6; i++;
	//5-1-6 / 6-1-2 (cara lateral 2) 
	/*
			6			2


			5			1

	*/
	_indices[i] = 4; i++;
	_indices[i] = 0; i++;
	_indices[i] = 5; i++;

	_indices[i] = 5; i++;
	_indices[i] = 0; i++;
	_indices[i] = 1; i++;
	//5-8-6/6-8-7 (darrera) 
	/*
			6			7


			5			8

	*/
	_indices[i] = 4; i++;
	_indices[i] = 7; i++;
	_indices[i] = 5; i++;

	_indices[i] = 5; i++;
	_indices[i] = 7; i++;
	_indices[i] = 6; i++;
	//2-3-6 / 6-3-7 (top) 
	/*
			6			7


			2			3

	*/
	_indices[i] = 1; i++;
	_indices[i] = 2; i++;
	_indices[i] = 5; i++;

	_indices[i] = 5; i++;
	_indices[i] = 2; i++;
	_indices[i] = 6; i++;

	//5-8-1 / 1-8-4 (Bot) 
	/*
		1			4


		5			8

	*/
	_indices[i] = 4; i++;
	_indices[i] = 7; i++;
	_indices[i] = 0; i++;

	_indices[i] = 0; i++;
	_indices[i] = 7; i++;
	_indices[i] = 3; i++;

	if (i > (6 * 2 * 3))
		cerr << "Error, he fet masses cares" << endl; 

}