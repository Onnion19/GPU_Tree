#include "LSystem.h"

LSystem * LSystem::_lSystem = nullptr;


LSystem * LSystem::init()
{
	if (_lSystem != nullptr)
	{
		cerr << "You have already initialized the L_System, please use get_System instead init()" << endl;
		return _lSystem; 
	}

	_lSystem = new LSystem();
	//cout << "Created an empty LSystem" << endl;

	return _lSystem;	
}

LSystem * LSystem::init(string filename)
{
	if (_lSystem != nullptr)
	{
		cerr << "You have already initialized the L_System, please use get_System instead init()" << endl;
		return _lSystem;
	}

	
	_lSystem = new LSystem();

	if (_lSystem->ReadFile(filename))//read from file succees
	{
		return _lSystem;
	}
	else//Read from file failed
	{
		return nullptr;
	}

}



bool LSystem::AddRule(char item, string replaced)
{
	return _Dictionary.AddRule(item, replaced);
}


void LSystem::NewAxiom(string axiom)
{
	if(!axiom.empty())
		_Axiom = axiom;
}

string LSystem::GenerateTree(int generations, bool Debug)
{
	_Tree = _StringTree.Generate(_Axiom, _Dictionary, generations, Debug);
	return _Tree;
}

void LSystem::GenerateTree(unsigned short * buffer, unsigned short * indiceBuffer, unsigned int generations , bool debug)
{
	_StringTree.GenerateToBuffer(buffer, indiceBuffer, _Axiom, _Dictionary, generations, debug);

	///Guardar-nos la informació dels troncs --> GPU Pass
}

int LSystem::GenerateTreesFromFile(unsigned short * buffer, unsigned int * indiceBuffer,string filename, unsigned int generations, bool debug)
{
	return _StringTree.GenerateMultipleToBuffer(buffer, indiceBuffer, filename, _Dictionary, generations, debug);
}

LSystem::~LSystem()
{
	Clear(); 
	_lSystem = nullptr;
}

vector<unsigned short> LSystem::TranslateCurrentTree(bool debug)
{
	return _StringTree.TranslateCurrentTree(_Dictionary,debug);
}

void LSystem::showItems()const
{
	_Dictionary.PrintAll();
}
//PRIVATE
LSystem::LSystem()
{
}
void LSystem::Clear()
{
	_Axiom = "";
	_Tree = "";
	_Dictionary.ClearAll();
}

bool LSystem::ReadFile(const string filename)
{
	ifstream input;
	input.open(filename);
	if (input.fail() || !input.is_open())
	{
		cerr << "Failed trying to open: " << filename << endl;
		return false;
	}

	char A;
	string B; 

	while (!input.eof())
	{
		input >> A;
		input >> B >> B;
		
		_Dictionary.AddRule(A, B);
	}

	return true;
}
