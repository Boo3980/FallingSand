#include "shaderClass.h"


std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);

	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}


Shader::Shader(const char* vertexfile, const char* fragfile) {


	std::string vertexcode, fragcode;
	vertexcode = get_file_contents(vertexfile);
	fragcode   = get_file_contents(  fragfile);

	const char* vertexshader = vertexcode.c_str();
	const char* fragshader   =   fragcode.c_str();

	glShaderSource(vertexshader, 1)
	//glAttachShader(ID, vertexshader);
	//glAttachShader(ID, fragshader);
}