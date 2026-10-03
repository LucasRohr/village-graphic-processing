#ifndef MINIMAP_H
#define MINIMAP_H

#include <vector>
#include <glad/glad.h>
#include "ObjetoCena.h"

// Desenha a vila vista de cima num quadrado no canto superior direito da tela
// (um "radar" de navegação), reaproveitando o mesmo shaderProgram da cena
// principal. Chamar depois de já ter desenhado a cena principal no frame
void desenhaMinimapa(const std::vector<ObjetoCena>& objetos, GLuint shaderProgram, int screenWidth, int screenHeight);

#endif
