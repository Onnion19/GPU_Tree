#include "model.h"
#include <typeinfo>
model::model() {

}

model::model(const model &)
{

}

bool model::loadModel(string path,bool debug)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return false;
	}
	directory = path.substr(0, path.find_last_of('/'));

	ProcessNodeStructure(scene->mRootNode, scene);

	return true;
}

void model::ProcessNodeStructure(aiNode *node, const aiScene * scene) {

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		ProcessNodeStructure(node->mChildren[i], scene);
}


Mesh model::processMesh(aiMesh * mesh, const aiScene * scene) {
	vector<Vertex> vertices; 
	vector<unsigned int> indices; 
	vector<Texture> textures;
	float min_x = 0, max_x = 0; 
	float min_y = 0, max_y = 0;
	float min_z = 0, max_z = 0;
	//Vertex Data
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		//Vertex
		Vertex vertex; glm::vec3 vect; 
		vect.x = mesh->mVertices[i].x; 
		vect.y = mesh->mVertices[i].y;
		vect.z = mesh->mVertices[i].z;
		vertex._posicio = vect;
		//Agafar dades per la normalitzacio

		if (i != 0)
		{
			if (vect.x > max_x) max_x = vect.x;
			else if (vect.x < min_x) min_x = vect.x;
			else;

			if (vect.y > max_y) max_y = vect.y;
			else if (vect.y < min_y) min_y = vect.y;
			else;

			if (vect.z > max_z) max_z = vect.z;
			else if (vect.z < min_z) min_z = vect.z;
			else;

		}
		else
		{
			max_x = min_x = vect.x;
			max_y = min_y = vect.x;
			max_z = min_z = vect.x;
		}



		//Index
		vect.x = mesh->mNormals[i].x;
		vect.y = mesh->mNormals[i].y;
		vect.z = mesh->mNormals[i].z;
		vertex._normal = vect; 

		//TextureCoordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec; 
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex._texCoords = vec;
		}
		else
			vertex._texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	//Indice Data
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//Material Data

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	//Establir quins son els eixos mes grans
	float normFactor; 
	float x = (max_x - min_x), y = (max_y - min_y), z = (max_z - min_z);


	if (x > y)normFactor = x;
	else if (y > z) normFactor = y; 
	else normFactor = z;
	
		

	normalizeVertexAxis(vertices,min_x,min_y,min_z,normFactor,true);

	return Mesh(vertices, indices, textures);
}


void model::normalizeVertexAxis(vector<Vertex> &v, float minX, float minY, float minZ, float normFactor, bool debug)
{
	if (debug)
		cerr << "NORMALITZACIÓ DE VERTEXS:" << endl;
	for (int i = 0; i < v.size(); i++)
	{
		//Normalitzem l'objecte entre 0 - 1
		v[i]._posicio.x = ((v[i]._posicio.x - minX) / normFactor) ;
		v[i]._posicio.y = (v[i]._posicio.y - minY) / normFactor;
		v[i]._posicio.z = (v[i]._posicio.z - minZ) / normFactor;

		//Pasem l'objecte a -0.5 i 0.5 per tenir el centre al 0
		v[i]._posicio.x -= 0.5;
		v[i]._posicio.y -= 0.5;
		v[i]._posicio.z -= 0.5;

		if (debug)
			cerr << "\tVertex " << i << ": ( " << v[i]._posicio.x << " , " << v[i]._posicio.y << " , " << v[i]._posicio.z << " )" << endl;
		
	}

	if (debug)
		cerr << "FI NORMALITZACIO DE VERTEXS" << endl;
}

vector<Texture> model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = TextureFromFile(str.C_Str(), directory);
		texture.type = typeName;
		texture.path = str;
		textures.push_back(texture);
	}
	return textures;
}

bool model::setBuffers()
{
	bool ok = true; 
	for (unsigned i = 0; i < _meshes.size(); i++)/*MOST MODELS ONLY HAVE 1 MESH, BUT JUST IN CASE*/
		ok = ok &&_meshes[i].setBuffers();

	return ok;
}


void model::set_shaders(string vertexShader, string fragmentShader)
{
	for (unsigned i = 0; i < _meshes.size(); i++)/*MOST MODELS ONLY HAVE 1 MESH, BUT JUST IN CASE*/
		_meshes[i].set_shaders(vertexShader, fragmentShader);
}
void model::draw()
{
	for (unsigned i = 0; i < _meshes.size(); i++)/*MOST MODELS ONLY HAVE 1 MESH, BUT JUST IN CASE*/
		_meshes[i].draw();
}
	


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}