#include "ShaderCompiler.h"

ShaderCompiler::ShaderCompiler()
{
	init(); 
}

ShaderCompiler::ShaderCompiler(string vshader, string fshader)
{
	set_shaders(vshader, fshader);
}

void ShaderCompiler::init()
{
	_ok = false; 
	_vertexShader = _fragmentShader = "";
	_IDvShader = _IDfShader = _IDProgram = 0;
}

void ShaderCompiler::set_shaders(string vshader, string fshader)
{
	cerr << "Setting shader paths on ShaderCompiler: " << vshader << " / " << fshader << endl;
	_vertexShader = vshader;
	cerr << "vshader val: " << _vertexShader << endl;
	_fragmentShader = fshader;
	compile_shaders();
}

bool ShaderCompiler::compile_shaders()
{
	if (_IDvShader != 0 || _IDfShader != 0)//Si els hem compilat anteriorment, els destruim i tornem a generar
		clearShader();

	_IDvShader = glCreateShader(GL_VERTEX_SHADER);//Creem el shader pel vshader
	_IDfShader = glCreateShader(GL_FRAGMENT_SHADER);//Creem el shader pel fshader

	if (_IDvShader == 0)//Comprovem que estigui correcte
	{
		cerr << "Error amb glCreateShader del vertex shader" << endl; 
		return false; 
	}

	//Compilar el vertexShader
	const GLchar * VertexSource = fileToChar(_vertexShader);
	glShaderSource(_IDvShader, 1,&VertexSource , NULL);

	if (_IDvShader == 0)
		cerr << "Error en glShaderSource, falla en agafar el VERTEX" << endl;
	glCompileShader(_IDvShader);
	_ok = checkCompile(_IDvShader);

	//Compilar el fragment Shader
	const GLchar * FragmentSource = fileToChar(_fragmentShader);
	glShaderSource(_IDfShader, 1, &FragmentSource, NULL);
	if (_IDfShader == 0)
		cerr << "Error en glShaderSource, falla en agafar el FRAGMENT" << endl;
	glCompileShader(_IDfShader);
	_ok = _ok && checkCompile(_IDfShader);

	cerr << "El valor de la variable es: " << _vertexShader << " Compilat al: " << _IDvShader << endl;
	cerr << "El valor de la variable es: " << _fragmentShader << " Compilat al: " << _IDfShader << endl;


	//Crear i linkar el program shader 
	_IDProgram = glCreateProgram();
	glAttachShader(_IDProgram, _IDvShader);
	glAttachShader(_IDProgram, _IDfShader);
	glLinkProgram(_IDProgram);
	_ok = _ok && checkLinking(_IDProgram);

	glUseProgram(_IDProgram);

	return _ok;
}

void ShaderCompiler::clearShader() {
	if(_IDfShader!=0)
		glDeleteShader(_IDfShader);
	if(_IDvShader!= 0)
		glDeleteShader(_IDvShader);
	_IDfShader = _IDvShader = 0;
}



char *ShaderCompiler::fileToChar(string filepath)const {
	cerr << "Intentant crear el buffer de : " << filepath << endl;
	FILE *file;
	file = fopen(filepath.c_str(), "rb");
	unsigned int length;

	char *buffer = nullptr;
	if (file) {
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		buffer = (char *)malloc(length + 1);
		fseek(file, 0, SEEK_SET);
		fread(buffer, length, 1, file);
		fclose(file);
		buffer[length] = '\0';
	}
	else
		cerr << "Ha fallat en obrir el fitxer: " << filepath << endl;

	return buffer;
}

unsigned int ShaderCompiler::getProgram()const
{
	return _IDProgram;
}

bool ShaderCompiler::checkCompile(unsigned int id)
{
	int succes;
	char info[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		if (id == _IDvShader)
			cerr << "Error en compilar vertex shader:" << endl;
		else
			cerr << "Error en compilar fragment shader" << endl;
		glGetShaderInfoLog(id, 512, NULL, info);
		cerr << info << endl;
		return false;
	}
	return true;
}

bool ShaderCompiler::checkLinking(unsigned int id)
{
	int succes;
	char info[512];
	glGetProgramiv(id, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		cerr << "Error en linkar el shader:" << endl;
		glGetProgramInfoLog(id, 512, NULL, info);
		cerr << info << endl;
		return false;
	}
}