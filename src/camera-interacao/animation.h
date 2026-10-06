#ifndef ANIMATION_H
#define ANIMATION_H

#include <glm/glm.hpp>

// Ângulo atual (em graus), em função do tempo decorrido, para a rotação da pá
// do moinho em torno do seu próprio eixo
float calculaAnguloPa(float tempo);

// Posição animada da nuvem: desloca continuamente no eixo X a partir da posição
// inicial e "retorna" de volta para dentro de [-limiteX, limiteX) ao sair da
// faixa de limite, criando um ciclo (a nuvem reaparece do lado oposto, do começo)
glm::vec3 calculaPosicaoNuvem(const glm::vec3& posInicial, float tempo, float velocidade = 0.6f, float limiteX = 18.0f);

#endif
