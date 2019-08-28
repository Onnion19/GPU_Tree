#pragma once 

//Local Headers
#include "Dictionary.h"
// System Headers
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <forward_list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class LSystemMath {

public: 
	/**
	* Matrix Operation Buffer.
	* Given a dictionary it creates and fill a buffer with all the matrix operation. The order of the operations is the same as the L-System simbols in the dictionary. 
	*/
	void GenerateMatrixOperationBuffer(glm::mat4 * buffer, Dictionary dic, bool debug = false) const; 
	
	
	
	
	
private: 

	const float ROTATION = 0.26f;
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