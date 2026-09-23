#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <glad/glad.h>

// Lê o conteúdo de um arquivo .glsl do disco e devolve como string
std::string leShaderDoArquivo(const char* caminho);

// Compila o vertex e o fragment shader a partir dos caminhos dos arquivos .glsl,
// linka os dois num único shader program e retorna o ID pronto para uso em glUseProgram
GLuint compilaShaderProgram(const char* vertexPath, const char* fragmentPath);

#endif
