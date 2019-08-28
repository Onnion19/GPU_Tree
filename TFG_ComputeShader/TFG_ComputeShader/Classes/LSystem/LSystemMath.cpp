#include "LSystemMath.h"


void LSystemMath::GenerateMatrixOperationBuffer(glm::mat4 * buffer, Dictionary dic, bool debug)const
{
	int i = 0; 
	buffer[i] = glm::mat4(1.0f); i++;
	buffer[i] = glm::mat4(1.0f); i++;
	buffer[i] = glm::mat4(1.0f); i++;
	buffer[i] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, TRANSLATE_RTAIO, 0.0f)); i++;
	buffer[i] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, TRANSLATE_RTAIO, 0.0f)); i++;
	buffer[i] = glm::rotate(glm::mat4(1.0f), ROTATION, glm::vec3(1.0f, 0.0f, 0.0f));; i++;
	buffer[i] = glm::rotate(glm::mat4(1.0f), -ROTATION, glm::vec3(1.0f, 0.0f, 0.0f)); i++;
	buffer[i] = glm::rotate(glm::mat4(1.0f), ROTATION, glm::vec3(0.0f, 1.0f, 0.0f)); i++;
	buffer[i] = glm::rotate(glm::mat4(1.0f), -ROTATION, glm::vec3(0.0f, 1.0f, 0.0f)); i++;
	buffer[i] = glm::rotate(glm::mat4(1.0f), ROTATION, glm::vec3(0.0f, 0.0f, 1.0f)); i++;
	buffer[i] = glm::rotate(glm::mat4(1.0f), -ROTATION, glm::vec3(0.0f, 0.0f, 1.0f)); i++;
	
	for (int j = 0; j < 12; j++)
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