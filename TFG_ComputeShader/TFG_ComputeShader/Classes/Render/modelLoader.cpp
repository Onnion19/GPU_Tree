#include "modelLoader.h"



modelLoader::modelLoader()
{
	scene = nullptr;
}

modelLoader::modelLoader(const std::string file)
{
	scene = nullptr; 
	loadObject(file);
}


modelLoader::~modelLoader()
{
}

const aiScene * modelLoader::loadObject(const std::string file, bool debug)
{
	//scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	scene = importer.ReadFile(file,aiProcess_Triangulate);
	if (scene == nullptr)
		std::cerr << "EL MODEL NO S'HA CARRGAT CORRECTAMENT" << std::endl;
	else 
		std::cout << "Has carregat: " << scene->mNumMeshes << std::endl; 

	if (debug)
	{
		if (scene != nullptr)
		{
			std::cerr << "Informacio del model: " << std::endl;
			std::cerr << "\t" << "Num meshes: " << scene->mNumMeshes << std::endl;
			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				std::cerr << "\t" << "Mesh: " << i + 1 << std::endl;
				std::cerr << "\t" << "\t" << "Vertices: " << scene->mMeshes[i]->mNumVertices << std::endl;
				std::cerr << "\t" << "\t" << "Faces: " << scene->mMeshes[i]->mNumFaces << std::endl;

				std::cerr << "\t" << "\t" << "END OF MESH" << std::endl << std::endl;
			}
		}
		else
			std::cerr << "ERROR WHILE LOADING SCENE: " << getError()<<std::endl;

	}
	return scene;

}

std::string modelLoader::getError()const
{
	return importer.GetErrorString();
}