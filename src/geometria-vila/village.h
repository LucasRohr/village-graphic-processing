#ifndef VILLAGE_H
#define VILLAGE_H

#include <vector>
#include <glm/glm.hpp>
#include "ObjetoCena.h"
#include "texture.h"

// Monta todos os objetos "de construção" da vila (terreno, casas, celeiros,
// moinhos, árvores, fardos, nuvens) e os acrescenta ao vetor objetos. Os
// animais .obj são adicionados depois, por obj_loader.h (carregaAnimais)
void geraVila(std::vector<ObjetoCena>& objetos, const TexturasVila& tex);

// Matriz Model "padrão" de um objeto: translação até sua posição, rotação em
// Y e escala, nessa ordem. Usada por quase todos os objetos; os que têm
// animação própria (pá do moinho, nuvem) montam a matriz manualmente em main.cpp
glm::mat4 calculaModelBase(const ObjetoCena& obj);

#endif
