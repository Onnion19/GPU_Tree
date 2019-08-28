#include "Mesh.h"


Mesh::Mesh(vector<Vertex> vertex, vector<unsigned int> indices, vector<Texture> textures)
{
	_vertices = vertex; 
	_indices = indices; 
	_textures = textures;
}

Mesh::Mesh(const Mesh &m)
{
	_vertices = m._vertices;
	_indices = m._indices; 
	_textures = m._textures;
	_sc = m._sc;
	_IDVao = m._IDVao;
	_IDVbo = m._IDVbo; 
	_IDVeo = m._IDVeo;
}

bool Mesh::loadMesh(string path, bool debug)
{
	//DO NOT USE LOAD MESH, THE CONSTRUCTOR EXPECTS TO HAVE ALL THE DATA OF A LOADED MESH, SO THERE IS NO POINT TO LOAD A NEW MESH HERE
	return true;
}


bool Mesh::setBuffers()
{
	/*Tutorial complete buffers https://learnopengl.com/Model-Loading/Model i https://learnopengl.com/Model-Loading/Mesh*/
	glGenVertexArrays(1, &_IDVao);
	glGenBuffers(1, &_IDVbo);
	glGenBuffers(1, &_IDVeo);

	glBindVertexArray(_IDVao);
	glBindBuffer(GL_ARRAY_BUFFER, _IDVbo);

	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IDVeo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

	//Vertex 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _normal));
	//Coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _texCoords));

	glBindVertexArray(0);

	return true;
}


void Mesh::draw()
{
	glBindVertexArray(_IDVao);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void Mesh::set_shaders(string vertexShader, string fragmentShader)
{
	_sc.set_shaders(vertexShader, fragmentShader);
}
