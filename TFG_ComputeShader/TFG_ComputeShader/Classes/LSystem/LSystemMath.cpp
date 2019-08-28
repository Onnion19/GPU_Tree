#include "LSystemMath.h"




void LSystemMath::GenerateMatrixOperationBuffer(glm::mat4 * buffer, Dictionary dic, bool debug)const
{

	//In order to know the order of the matrix, check the symbol file ("Simbols.txt") The order of symbols are the same order as matrix to be able to create a direct relation. 
	int i = 0; 
	/*
	* First 3 Matrices corresponds to None Matrix Translation but L-System data management, so will create neutral Matrix (Identity)
	*/
	buffer[i] = glm::mat4(1.0f); i++;
	buffer[i] = glm::mat4(1.0f); i++;
	buffer[i] = glm::mat4(1.0f); i++;
	/*
	* Following 2 Matrix Correspons to Move Forward, so creates translation matrix in Y axis (by default tree will grow in Y axis) 
	*/
	buffer[i] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, TRANSLATE_RTAIO, 0.0f)); i++;
	buffer[i] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, TRANSLATE_RTAIO, 0.0f)); i++;
	/*
	* Following 6 Matrixs are rotations in all 3 axes, from X, Y , Z. All have positive and negative rotation. 
	*/

	//X
	buffer[i] = glm::rotate(glm::mat4(1.0f), ROTATION, glm::vec3(1.0f, 0.0f, 0.0f)); i++;
	buffer[i] = glm::rotate(glm::mat4(1.0f), -ROTATION, glm::vec3(1.0f, 0.0f, 0.0f)); i++;
	//Y
	buffer[i] = glm::rotate(glm::mat4(1.0f), ROTATION, glm::vec3(0.0f, 1.0f, 0.0f)); i++;
	buffer[i] = glm::rotate(glm::mat4(1.0f), -ROTATION, glm::vec3(0.0f, 1.0f, 0.0f)); i++;
	//Z
	buffer[i] = glm::rotate(glm::mat4(1.0f), ROTATION, glm::vec3(0.0f, 0.0f, 1.0f)); i++;
	buffer[i] = glm::rotate(glm::mat4(1.0f), -ROTATION, glm::vec3(0.0f, 0.0f, 1.0f)); i++;
	

	//Further implementation for new translations, shear, or other transformations. At this moment leave them as Identity Matrix for consistency. 
	// If they won't be implemented, this 20 Identity matrix can be deleted. 
	for (int j = 0; j < 20; j++)
	{
		buffer[j + i] = glm::mat4(1.0f);
	}


	if (debug)
	{
		cerr << "==================================================";
		cerr << "Showing Buffer Math Matrix Operation: " << endl; 
		for (int index = 0; index < 12 + i; index++)
		{
			cerr << "\t Matrix number " << index << " :" << endl;
			for (int i = 0; i < 4; i++)
			{
				cerr << "\t\t    ";
				for (int j = 0; j < 4; j++)
				{
					cerr << buffer[index][j][i] << "\t\t";
				}
				cerr << endl;
			}
		}
		cerr << "==================================================";

	} 

}