#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <glad/glad.h>
#include <vector>
#include <string>
#include "ObjetoCena.h"

// Lê um arquivo .obj e cria o VAO correspondente.
// Layout de saída igual ao de primitives.h (posição vec3 + UV vec2 intercalados), para funcionar com o mesmo shader da vila.
GLuint carregaOBJ(const std::string& filePath, int& nVertices);

// Carrega os modelos de animais (Cube Pets) e os adiciona à cena como
// ObjetoCena do tipo ANIMAL, todos compartilhando a mesma textura (colormap.png)
void carregaAnimais(std::vector<ObjetoCena>& objetos);

#endif
