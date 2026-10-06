#ifndef MINIMAP_H
#define MINIMAP_H

#include <vector>
#include <glad/glad.h>
#include "ObjetoCena.h"

// Desenha a vila vista de cima num quadrado no canto superior direito da tela
// reaproveitando o mesmo shaderProgram da cena principal
void desenhaMinimapa(const std::vector<ObjetoCena>& objetos, GLuint shaderProgram, int screenWidth, int screenHeight);

#endif
