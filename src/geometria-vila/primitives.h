#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glad/glad.h>

// Cada função gera VAO/VBO com posição (vec3, location 0) e UV (vec2, location 1)
// intercalados, e devolve o total de vértices em nVertices, para uso no glDrawArrays.

// Caixa retangular (6 faces). largura = extensão em X, altura = extensão em Y,
// profundidade = extensão em Z. Com os 3 parâmetros em 1.0f (default), gera um
// cubo de 1x1x1 centrado na origem -- as demais construções da vila (casa,
// celeiro) escalam essas dimensões para cada peça
GLuint criaVAOCubo(int& nVertices, float largura = 1.0f, float altura = 1.0f, float profundidade = 1.0f);

// Retângulo no plano XZ (Y=0), usado como chão/terreno. largura = extensão em
// X, profundidade = extensão em Z
GLuint criaVAOPlano(int& nVertices, float largura = 1.0f, float profundidade = 1.0f);

// Pirâmide de base quadrada (telhados e copa de árvore). baseTamanho = lado da
// base quadrada, altura = distância da base até o ápice
GLuint criaVAOPiramide(int& nVertices, float baseTamanho = 1.0f, float altura = 1.0f);

// Cilindro (torres, troncos, fardos de feno) aproximado por um polígono de N
// lados. segmentos = número de fatias ao redor da circunferência (quanto
// maior, mais "redondo" e mais vértices); raio e altura em unidades do mundo
GLuint criaVAOCilindro(int& nVertices, int segmentos = 20, float raio = 0.5f, float altura = 1.0f);

// Pá do moinho em cruz (duas tábuas cruzadas a 90°) num único mesh, no plano XY
// local. O render loop gira esse objeto em torno do próprio eixo Z (não do Y,
// que é usado por calculaModelBase) para simular a rotação da pá.
// comprimento = tábua maior (ponta a ponta), largura = tábua menor, espessura = grossura (eixo Z)
GLuint criaVAOPaMoinho(int& nVertices, float comprimento = 1.6f, float largura = 0.15f, float espessura = 0.06f);

#endif
