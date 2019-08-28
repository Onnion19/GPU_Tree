#pragma once
// Local Headers
#include "modelLoader.h"
#include "Meshes/Triangle.h"
#include "renderSystem.h"
#include "Meshes/Cube.h"
#include "Meshes/model.h"
// System Headers
#include <vector>
#include <forward_list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
/**
* Class to render CPU tree.
* A class that wrapp all the models and geometry calculations to render an L-System tree. 
*/
class TreeRenderer {
public: 
	/**
	* Default constructor. 
	* It creates a tree of 1 box as a root. 
	*/
	TreeRenderer();

	
	TreeRenderer(vector<glm::mat4> transforms);
	/**
	* Parametric constructor.
	* Just safe the string as a tree to render. (Caution, tree is not calculated @see render() for more information. 
	* @param <string> tree in L-System writting. 
	*/
	TreeRenderer(string tree);
	void render();//Calcula les noves transformacions per cada cub, i envia les matrius al sistema de render. 
private: 

	struct matrixUtils {
	public: 
		
		matrixUtils() { current_matrix = glm::mat4(1.0f); }
		glm::mat4 get_currentMatrix(){ return current_matrix; }

		//operacions per la matriu actual: 
		void forward(){ current_matrix =  current_matrix* forward_matrix; }

		void rotate_x_positive() { current_matrix = current_matrix * Positive_XAxisRotation; }
		void rotate_y_positive() { current_matrix = current_matrix * Positive_YAxisRotation; }
		void rotate_z_positive() { current_matrix = current_matrix * Positive_ZAxisRotation; }

		void rotate_x_negative() { current_matrix = current_matrix * Negative_XAxisRotation; }
		void rotate_y_negative() { current_matrix = current_matrix * Negative_YAxisRotation; }
		void rotate_z_negative() { current_matrix = current_matrix * Negative_ZAxisRotation; }
		
		void scale() { current_matrix = current_matrix * scale_matrix; }
		//Operacions pel stack.
		void queue() { stack_matrix.push_front(current_matrix);}//Apila la matriu actual
		void dequeue() {//Elimina l'ultima matriu apilada
			if (!stack_matrix.empty())
			{
				current_matrix = stack_matrix.front();
				stack_matrix.pop_front();
			}
		}
		
		//debug
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
		forward_list<glm::mat4> stack_matrix;
		glm::mat4 current_matrix;//MATRIU ACTUAL EN CADA MOMENT

	 ///CONSTANTS(MATRIUS DE TRANSFORMACIO BASIQUES)
		const float ROTATION = 0.15f;//45º = 0.79
		const float SCALE_RATIO = 0.9f;
		const float TRANSLATE_RTAIO = 1.2f;

		const glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(SCALE_RATIO, SCALE_RATIO, SCALE_RATIO));

		const glm::mat4 forward_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, TRANSLATE_RTAIO, 0.0f));

		const glm::mat4 Positive_XAxisRotation = glm::rotate(glm::mat4(1.0f), ROTATION, glm::vec3(1.0f, 0.0f, 0.0f));
		const glm::mat4 Positive_YAxisRotation = glm::rotate(glm::mat4(1.0f), ROTATION, glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 Positive_ZAxisRotation = glm::rotate(glm::mat4(1.0f), ROTATION, glm::vec3(0.0f, 0.0f, 1.0f));

		const glm::mat4 Negative_XAxisRotation = glm::rotate(glm::mat4(1.0f), -ROTATION, glm::vec3(1.0f, 0.0f, 0.0f));
		const glm::mat4 Negative_YAxisRotation = glm::rotate(glm::mat4(1.0f), -ROTATION, glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 Negative_ZAxisRotation = glm::rotate(glm::mat4(1.0f), -ROTATION, glm::vec3(0.0f, 0.0f, 1.0f));
	};
	
	matrixUtils _matrixUtils;
	string _tree; //String que guardara l'arbre
	renderSystem _render;//Sistem de renderitzat
	Cube * _cube = nullptr; //Cub per defecte que s'utlitza per posar geometria
	Cube * _flower = nullptr; //Cub per posar el model
	model * _tronc = nullptr;
	vector<glm::mat4>_transforms; //Vector que par cada cub guarda la seva transformacio (ordenats segons string d'arbe (_tree) 
	void init();
	void initCube();
	void initModel();
	void initFlower();

	/*This function converts L-System Data to Matrix data, using the algorithm of tortoise. THIS FUNCTIONS IS FOR CPU CALCULATIONS*/
	void calculateTransforms();



};