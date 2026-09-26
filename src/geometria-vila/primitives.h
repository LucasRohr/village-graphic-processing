#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glad/glad.h>

// Cada função gera VAO/VBO com posição (vec3, location 0) e UV (vec2, location 1)
// intercalados, e devolve o total de vértices em nVertices, para uso no glDrawArrays.
GLuint criaVAOCubo(int& nVertices, float largura = 1.0f, float altura = 1.0f, float profundidade = 1.0f);
GLuint criaVAOPlano(int& nVertices, float largura = 1.0f, float profundidade = 1.0f);
GLuint criaVAOPiramide(int& nVertices, float baseTamanho = 1.0f, float altura = 1.0f);
GLuint criaVAOCilindro(int& nVertices, int segmentos = 20, float raio = 0.5f, float altura = 1.0f);

// Pá do moinho em cruz (duas tábuas cruzadas a 90°) num único mesh, no plano XY
// local. O render loop gira esse objeto em torno do próprio eixo Z (não do Y,
// que é usado por calculaModelBase) para simular a rotação da pá.
GLuint criaVAOPaMoinho(int& nVertices, float comprimento = 1.6f, float largura = 0.15f, float espessura = 0.06f);

#endif