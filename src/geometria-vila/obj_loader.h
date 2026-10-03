#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <glad/glad.h>
#include <vector>
#include <string>
#include "ObjetoCena.h"

// Carrega um .obj com triangulação em leque (suporta faces com 3+ vértices) e
// devolve o VAO pronto, com posição (vec3) + UV (vec2) intercalados -- mesmo
// layout usado pelas primitivas, pra funcionar com o mesmo shader.
GLuint carregaOBJ(const std::string& filePath, int& nVertices);

// Carrega os modelos de animais (Cube Pets) e os adiciona à cena como
// ObjetoCena do tipo ANIMAL, cada um com sua própria textura.
void carregaAnimais(std::vector<ObjetoCena>& objetos);

#endif