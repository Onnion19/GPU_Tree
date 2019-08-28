#include "Triangle.h"

Triangle::Triangle()
{
}

void Triangle::set_punts(glm::vec3 A, glm::vec3 B, glm::vec3 C)
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
	_punts.push_back(A);
	_punts.push_back(B);
	_punts.push_back(C);
}

bool Triangle::setBuffers()//Hauria de ser una altre classe
{
	//Creem el VAO	
	glGenVertexArrays(1, &idVAO);//Generem Vao
	glGenBuffers(1, &idVBO);//Generemo VBO

	glBindVertexArray(idVAO);//Bind del vertex Array
	glBindBuffer(GL_ARRAY_BUFFER,idVBO);//Bind del vertx object

	//glBufferData(GL_ARRAY_BUFFER, sizeof(_points), _points, GL_STATIC_DRAW);//Enviem les dades
	glBufferData(GL_ARRAY_BUFFER, _punts.size()*sizeof(glm::vec3), &_punts[0], GL_STATIC_DRAW);//Enviem les dades
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//Expliquem com agafar les dades
	glEnableVertexAttribArray(0);
	if (idVAO + idVBO < 2)
	{
		cerr << "ALGUNA COSA HA FALLAT"<<endl;
	}
	else
		cout << "TOT HA ANAT Be" << endl;
	return true; 
}

void Triangle::set_shaders(string vshader, string fshader)
{
	_sc.set_shaders(vshader, fshader);
}


unsigned int Triangle::get_fShader()const
{
	return _sc.get_fShader();
}

unsigned int Triangle::get_vShader()const
{
	cerr << "Vshader del triangle: " << _sc.get_vShader() << endl;
	return _sc.get_vShader();
}


void Triangle::updateFrame() {
	_sc.compile_shaders();//No tinc clar que faci falta, ja que el programa dins de la gpu no s'esborra fins que acaba l'app
}

void Triangle::draw()
{
	glBindVertexArray(idVAO); 
	glDrawArrays(GL_TRIANGLES, 0, 3);
}