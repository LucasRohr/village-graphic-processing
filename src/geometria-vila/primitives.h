#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glad/glad.h>

// Cada função gera VAO/VBO com posição (vec3, location 0) e UV (vec2, location 1)
// intercalados, e devolve o total de vértices em nVertices, para uso no glDrawArrays.

// Cubo usado como base para casas e celeiros
GLuint criaVAOCubo(int& nVertices, float largura = 1.0f, float altura = 1.0f, float profundidade = 1.0f);

// Retângulo no plano XZ (Y=0), usado como chão/terreno
GLuint criaVAOPlano(int& nVertices, float largura = 1.0f, float profundidade = 1.0f);

// Pirâmide de base quadrada (usado em telhados e copa de árvore)
GLuint criaVAOPiramide(int& nVertices, float baseTamanho = 1.0f, float altura = 1.0f);

// Cilindro (torres, troncos, fardos de feno) aproximado por um polígono de N lados
// Segmentos = número de fatias ao redor da circunferência (quanto maior, mais "redondo" e mais vértices)
GLuint criaVAOCilindro(int& nVertices, int segmentos = 20, float raio = 0.5f, float altura = 1.0f);

// Pá do moinho em cruz (duas tábuas cruzadas a 90°) num único mesh, no plano XY
GLuint criaVAOPaMoinho(int& nVertices, float comprimento = 1.6f, float largura = 0.15f, float espessura = 0.06f);

#endif