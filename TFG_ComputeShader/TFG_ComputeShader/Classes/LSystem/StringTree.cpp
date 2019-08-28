#include "StringTree.h"



StringTree::StringTree()
{
}



string StringTree::Generate(string Axiom, Dictionary Dic, int generations,bool Debug)//Using a vector for insert and deleting from the middle, is it worth? 
{

	_tree = _axiom = Axiom; 
	string newTree; 

	for (int i = 0; i < generations; i++)//n^2, crec que es pot reduir calculant ja la traduccio final.
	{
		if (Debug)
		{
			cerr << "Generation: " << i << endl;
			cerr << "\t Starting Root: " << _tree << endl;
		}

		for (int symbol = 0; symbol < _tree.size(); symbol++)
		{
			
			string aux = Dic.Replace(_tree[symbol]);
			if (Debug)
			{
				cerr << "Replacing: " << _tree[symbol] << endl;
				cerr << "\t Rule founded: " << _tree[symbol] << " --> " << aux << endl;
				cerr << "\t Result: " << newTree << " + " << aux << endl;
			}

			newTree += aux; 	
		}

		_tree = newTree;
		newTree = "";
		if (Debug)
		{
			cerr << "Generation result: " << _tree << endl;
			cerr << "=====================" << endl;
		}
	}

	return _tree;
}

vector<unsigned short> StringTree::TranslateCurrentTree(Dictionary dic, bool debug)
{
	return dic.TranslateStringToShort(_tree, debug);
}



/*							ARRAY STRUCTURE

We have diferent rows and columns: 

	each row = 1 branch.
	each column = 1 Item form lSystem langauge to create the branch. 

	IMPORTANT: THE FIRST POSITION OF EVERY BRANCHES, IS USED TO INDICATE THE DEPTH OF THE BRANCH.

	The depht of the brach is determined in LSystem::MAX_WIDTH. If a branches it's larger, the "Extra" items are ignored. 
	
	Symbol ' . ' or ' [ ' or ' ] ' determines where the branch finishes: 
		- ' . '  : The branch has no new branches inside and it finishes at position ' n '. 
		- ' [ ' : The branch creates a new branch, so all the rest are treated as a new branches. 
		- ' ] ' : The branch was a Sub-Branch of another one and it has ended. 


	Whenever we Replace on character for it's translate, if there are any new branch, we take an extra row to write that branch. 

	Each generation uses 2 buffers: 
		1 - The original one, which can be the axiom or the previous generation. 
		2 - The new one, starts empty and it's getting filled as long as we start translating the first buffer. 

	Finally, we copy the result buffer to user's and send it. 


	So an example may be: 

	1 - BRANCH 0.0		DEPTH: 0 BRANCH: XFFAEIOFISFA[
	2 - BRANCH 0.1		DEPTH: 1 BRANCH: UIFJEINF]
	3 - BRANCH 0.2		DEPTH: 1 BRANCH: UIEWIF[
	4 - BRANCH 0.2.1	DEPTH: 2 BRANCH: EIFIIFE]
	5 - BRANCH 0.2.1.1	DEPTH: 3 BRANCH: SFE]
	6 - BRANCH 0.3.0	DEPTH: 1 BRANCH: IOJJFIEUHFEJIFUEHUHFEUY] (HEUFYGBFUE) -->  BRANCH WAS TO LARGE SO " HEUFYGBFUE " WAS IGNORED
*/


void StringTree::GenerateToBuffer(unsigned short * buffer, unsigned short * indiceBuffer, string Axiom, Dictionary dic, unsigned int generations , bool debug )
{
	unsigned int nextPositionToWrite = 0;
	//Process Axiom 
	nextPositionToWrite = ProcessAxiom(buffer, nextPositionToWrite,1,Axiom,dic);
	indiceBuffer[0] = nextPositionToWrite;

	if (debug)
	{
		cerr << "AXIOM PROCESSED: " << endl;
		cerr << "\t"; 
		for (int i = 0; i < indiceBuffer[0]; i++)
		{
			if (i%Config::MAX_WIDTH == 0)cerr << endl<<endl;
			if (buffer[i] != 0)
				cerr << buffer[i] << "  ";

		}
		cerr << endl;
	}
	
	unsigned int FirstPositionRead; 
	unsigned int LastPositionRead; 
	unsigned int currentgeneration = 1; 
	bool correct = true; 
	// Process Tree (Gen 0 == seed)
	while(correct && currentgeneration < generations)
	{
		if (debug) 
		{
			cerr << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
			cerr << "PROCESSING GENERATION: " << currentgeneration<<endl;
		}
			
		correct = ProcessNewGeneration(buffer, indiceBuffer, currentgeneration, dic, debug) > 0;
		if (debug)
		{
			cerr << "GENERATION PROCESSED: " << endl; 
			cerr << "Start point: " << indiceBuffer[currentgeneration - 1] << " End Point" << indiceBuffer[currentgeneration] << endl; 
			for (int i = indiceBuffer[currentgeneration-1]; i < indiceBuffer[currentgeneration]; i++)
			{
				if (i%Config::MAX_WIDTH == 0)cerr << endl << endl;
				if(buffer[i]!=0)
					cerr << buffer[i] << "  ";
			}
			cerr << "\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		}
		currentgeneration++;
	}
}


/*
	AXIOM IS NOT AN STRING, BUT VECTOR OF INT 
	MAKE HOLE TREE: 
		PROCESS ALL BRANCHES FOR EACH GENERATION. 
		READ FROM OLD GENERATION TO NEW GENERATION. 
		STORE INDICES WHEN GENERATION ENDS. 

*/
unsigned int StringTree::ProcessAxiom(unsigned short * buffer, unsigned int initalPositionWrite,unsigned short initalDepth, string Axiom, Dictionary _dic)
{
	unsigned short branches = 0;
	unsigned short currentDepth = initalDepth;
	unsigned int currentWritablePos = initalPositionWrite;
	buffer[currentWritablePos] = currentDepth;
	currentWritablePos++;

	bool brancaCorrecte = true; 

	for (int i = 0; i < Axiom.length(); i++)
	{
		if (Axiom[i] == '[') // Nova branca
		{
			if (GenerateNewBranch(buffer, &branches, &currentWritablePos, &currentDepth, initalPositionWrite))
				brancaCorrecte = true;
			else
				return 0; //No s'ha pogut crear la nova branca, (posiblement falta espai al buffer)
		}
		else if (Axiom[i] == ']') // Tanquem branca
		{
			currentDepth-=1;
			brancaCorrecte = false;
		}
		else // Omplim Branca
		{
			if (currentWritablePos < Config::MAX_WIDTH) // Tenim espai a la branca
			{

				if (!brancaCorrecte)//Si s'ha tancat una branca prèviament i no se'n ha obert cap -> forcem a crear una nova branca
				{
					if (GenerateNewBranch(buffer, &branches, &currentWritablePos, &currentDepth, initalPositionWrite))
						brancaCorrecte = true; 
					else
						return 0;//No s'ha pogut crear la nova branca, (posiblement falta espai al buffer)
				}

				unsigned short item = _dic.TranslateCharToShort(Axiom[i]);
				if (item != 0)
					buffer[branches * Config::MAX_WIDTH + initalPositionWrite + currentWritablePos] = item;

				currentWritablePos++;
			}
		}
	}

	return (branches+1) * Config::MAX_WIDTH + initalPositionWrite;
}

//Retorna si ha pogut crear una nova branca
bool StringTree::GenerateNewBranch(unsigned short * buffer, unsigned short * branches, unsigned int * currentWritablePos, unsigned short * currentDepth, unsigned int initalPositionWrite)
{
	
	*branches+=1;
	*currentDepth+=1;
	cerr << "\t\t\t CREATING NEW BRANCH" << endl; 
	cerr << "\t\t\t\t Info: " << endl; 
	cerr << "\t\t\t\t num branches: " << *branches << "\t Initial pos Offset" << initalPositionWrite + (*branches-1)*Config::MAX_WIDTH << "\t Max size: " << Config::MAX_BUFFER_SIZE << endl;
	if (*branches * Config::MAX_WIDTH + initalPositionWrite > Config::MAX_BUFFER_SIZE)//Passem el buffer
		return false; 
	buffer[*branches * Config::MAX_WIDTH + initalPositionWrite] = *currentDepth;
	*currentWritablePos = 1;
	return true;
}

unsigned int StringTree::ProcessNewGeneration(unsigned short * buffer, unsigned short * indiceBuffer, unsigned int currentGeneration, Dictionary _dic, bool debug)
{

	unsigned int num_branches = 0;
	unsigned int writtingPos = 0, readingPos = 0;

	(currentGeneration < 2) ? readingPos = 0 : readingPos = indiceBuffer[currentGeneration - 2];
	writtingPos = indiceBuffer[currentGeneration - 1];
	num_branches = (writtingPos - readingPos) / Config::MAX_WIDTH;
	
	if (debug)
	{
		cerr << "\tGeneration info: " << endl; 
		cerr << "\t\t Reading Start: " << readingPos << endl; 
		cerr << "\t\t Writting Start: " << writtingPos << endl;
		cerr << "\t\t Num Branches: " << num_branches << endl;;
	}
	for (int i = 0; i < num_branches; i++)
	{
		writtingPos = ProcessBranch(buffer, readingPos, writtingPos, _dic, debug);
		if (writtingPos == 0)
		{
			if (debug)
			{
				cerr << "ERROR AT PROCESSING BRANCH: " << endl; 
				cerr << "Generation: " << currentGeneration << "\t Branch Number" << i << endl;
				return 0;
			}
		}
		readingPos += Config::MAX_WIDTH; 
	}
	
	indiceBuffer[currentGeneration] = writtingPos;
	return writtingPos;
}

unsigned int StringTree::ProcessBranch(unsigned short * buffer, unsigned int readingBufferPos, unsigned int writtingBufferPos, Dictionary _dic, bool debug)
{

	unsigned int readingBufferoffset = readingBufferPos; //Position where we start to read from the original buffer
	unsigned short branchDepth = buffer[readingBufferoffset]; //Initial branch depth
	buffer[writtingBufferPos] = branchDepth;
	unsigned int index = 1;

	unsigned short branchCreated = 0;// Number of branch created

	unsigned int writtingIndex = 1; //WritingIndex in the new branch
	unsigned int writtingOffset = writtingBufferPos; // Where we start to write in the buffer

	bool correctBranch = true; //Are the branches ok? 
	if (debug)
	{
		cerr << endl;
		cerr << "\tInfo about Processing Branch: " << endl; 
		cerr << "\t\tStart read: " << readingBufferoffset << "\tStart Write" << writtingBufferPos << endl; 
		cerr << "\t\tInitial Depth" << branchDepth << endl << endl; 
	}


	while (index < Config::MAX_WIDTH && buffer[readingBufferoffset + index] != 0)
	{

		vector<unsigned short> translate = _dic.Replace(buffer[readingBufferoffset + index]); 

		unsigned short depth = branchDepth;

		for (int i = 0; i < translate.size(); i++)
		{
			if (translate[i] == 1)//New Branch
			{
				if (debug)
				{
					cerr << "\t\tCreating new branch: " << branchCreated << endl;
				}
				if (i != 0)
				{
					if (GenerateNewBranch(buffer, &branchCreated, &writtingIndex, &depth, writtingOffset))
						correctBranch = true;
					else
						return 0;//No s'ha pogut crear la nova branca, (posiblement falta espai al buffer)
				}
				else
					depth += 1;			
				correctBranch = true;
			}
			else if (translate[i] == 2)//Close branch
			{
				depth-=1;
				correctBranch = false;
			}
			else//Populate the branch
			{

				if (!correctBranch) 
				{
					if (GenerateNewBranch(buffer, &branchCreated, &writtingIndex, &depth, writtingOffset))
						correctBranch = true;
					else
						return 0;
				}

				if (writtingIndex < Config::MAX_WIDTH)
					buffer[branchCreated * Config::MAX_WIDTH + writtingIndex + writtingOffset] = translate[i];

				writtingIndex++;
			}
		}
		index++;


	}

	return (branchCreated + 1) * Config::MAX_WIDTH + writtingOffset;
}

