#include "animation.h"
#include <cmath>

float calculaAnguloPa(float tempo) {
    float velocidadeGraus = 120.0f; // graus por segundo
    return tempo * velocidadeGraus;
}

glm::vec3 calculaPosicaoNuvem(const glm::vec3& posInicial, float tempo, float velocidade, float limiteX) {
    glm::vec3 pos = posInicial;

    float faixa = 2.0f * limiteX; // largura total do intervalo [-limiteX, limiteX), logo multiplica por 2
    float x = posInicial.x + tempo * velocidade; // posição "crua", sem limite, só aumentando com o tempo

    // Traz x de volta para dentro do intervalo [-limiteX, limiteX) usando função de módulo:
    // ao sair de um lado da faixa, a nuvem "reaparece" do lado oposto, criando o loop.

    // fmod pode devolver negativo, por isso o ajuste com "+faixa" antes de aplicar o módulo de novo
    x = fmod(x + limiteX, faixa);
    if (x < 0.0f) x += faixa;
    pos.x = x - limiteX;

    return pos;
}
