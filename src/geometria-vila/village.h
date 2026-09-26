#ifndef VILLAGE_H
#define VILLAGE_H

#include <vector>
#include <glm/glm.hpp>
#include "ObjetoCena.h"
#include "texture.h"

void geraVila(std::vector<ObjetoCena>& objetos, const TexturasVila& tex);
glm::mat4 calculaModelBase(const ObjetoCena& obj);

#endif