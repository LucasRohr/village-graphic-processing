#include "primitives.h"
#include <vector>
#include <cmath>
#include <glm/glm.hpp>

static GLuint criaVAODeBuffer(const std::vector<float>& dados, int& nVertices) {
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, dados.size() * sizeof(float), dados.data(), GL_STATIC_DRAW);

    int stride = 5 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    nVertices = (int)(dados.size() / 5);
    return vao;
}

// Gera os 36 vértices (posição+UV) de uma caixa com meias-dimensões e offset
// dados, sem criar VAO -- permite combinar várias caixas num único mesh (pá)
static std::vector<float> verticesCubo(float hx, float hy, float hz, glm::vec3 offset) {
    std::vector<float> v = {
        -hx,-hy, hz, 0.0f,0.0f,  hx,-hy, hz, 1.0f,0.0f,  hx, hy, hz, 1.0f,1.0f,
         hx, hy, hz, 1.0f,1.0f, -hx, hy, hz, 0.0f,1.0f, -hx,-hy, hz, 0.0f,0.0f,
         hx,-hy,-hz, 0.0f,0.0f, -hx,-hy,-hz, 1.0f,0.0f, -hx, hy,-hz, 1.0f,1.0f,
        -hx, hy,-hz, 1.0f,1.0f,  hx, hy,-hz, 0.0f,1.0f,  hx,-hy,-hz, 0.0f,0.0f,
        -hx,-hy,-hz, 0.0f,0.0f, -hx,-hy, hz, 1.0f,0.0f, -hx, hy, hz, 1.0f,1.0f,
        -hx, hy, hz, 1.0f,1.0f, -hx, hy,-hz, 0.0f,1.0f, -hx,-hy,-hz, 0.0f,0.0f,
         hx,-hy, hz, 0.0f,0.0f,  hx,-hy,-hz, 1.0f,0.0f,  hx, hy,-hz, 1.0f,1.0f,
         hx, hy,-hz, 1.0f,1.0f,  hx, hy, hz, 0.0f,1.0f,  hx,-hy, hz, 0.0f,0.0f,
        -hx,-hy,-hz, 0.0f,0.0f,  hx,-hy,-hz, 1.0f,0.0f,  hx,-hy, hz, 1.0f,1.0f,
         hx,-hy, hz, 1.0f,1.0f, -hx,-hy, hz, 0.0f,1.0f, -hx,-hy,-hz, 0.0f,0.0f,
        -hx, hy, hz, 0.0f,0.0f,  hx, hy, hz, 1.0f,0.0f,  hx, hy,-hz, 1.0f,1.0f,
         hx, hy,-hz, 1.0f,1.0f, -hx, hy,-hz, 0.0f,1.0f, -hx, hy, hz, 0.0f,0.0f,
    };
    for (size_t i = 0; i < v.size(); i += 5) {
        v[i]   += offset.x;
        v[i+1] += offset.y;
        v[i+2] += offset.z;
    }
    return v;
}

GLuint criaVAOCubo(int& nVertices, float largura, float altura, float profundidade) {
    std::vector<float> v = verticesCubo(largura / 2.0f, altura / 2.0f, profundidade / 2.0f, glm::vec3(0.0f));
    return criaVAODeBuffer(v, nVertices);
}

GLuint criaVAOPlano(int& nVertices, float largura, float profundidade) {
    float hx = largura / 2.0f, hz = profundidade / 2.0f;
    std::vector<float> v = {
        -hx, 0.0f,-hz, 0.0f,0.0f,  hx, 0.0f,-hz, 1.0f,0.0f,  hx, 0.0f, hz, 1.0f,1.0f,
         hx, 0.0f, hz, 1.0f,1.0f, -hx, 0.0f, hz, 0.0f,1.0f, -hx, 0.0f,-hz, 0.0f,0.0f,
    };
    return criaVAODeBuffer(v, nVertices);
}

GLuint criaVAOPiramide(int& nVertices, float baseTamanho, float altura) {
    float hb = baseTamanho / 2.0f, hy = altura / 2.0f;
    float bfl[3] = {-hb,-hy, hb}, bfr[3] = { hb,-hy, hb};
    float btr[3] = { hb,-hy,-hb}, btl[3] = {-hb,-hy,-hb};
    float ap[3]  = { 0.0f, hy, 0.0f};

    std::vector<float> v = {
        bfl[0],bfl[1],bfl[2], 0.0f,0.0f,  bfr[0],bfr[1],bfr[2], 1.0f,0.0f,  ap[0],ap[1],ap[2], 0.5f,1.0f,
        bfr[0],bfr[1],bfr[2], 0.0f,0.0f,  btr[0],btr[1],btr[2], 1.0f,0.0f,  ap[0],ap[1],ap[2], 0.5f,1.0f,
        btr[0],btr[1],btr[2], 0.0f,0.0f,  btl[0],btl[1],btl[2], 1.0f,0.0f,  ap[0],ap[1],ap[2], 0.5f,1.0f,
        btl[0],btl[1],btl[2], 0.0f,0.0f,  bfl[0],bfl[1],bfl[2], 1.0f,0.0f,  ap[0],ap[1],ap[2], 0.5f,1.0f,
        bfl[0],bfl[1],bfl[2], 0.0f,0.0f,  btr[0],btr[1],btr[2], 1.0f,1.0f,  bfr[0],bfr[1],bfr[2], 1.0f,0.0f,
        bfl[0],bfl[1],bfl[2], 0.0f,0.0f,  btl[0],btl[1],btl[2], 0.0f,1.0f,  btr[0],btr[1],btr[2], 1.0f,1.0f,
    };
    return criaVAODeBuffer(v, nVertices);
}

GLuint criaVAOCilindro(int& nVertices, int segmentos, float raio, float altura) {
    std::vector<float> v;
    float yBase = -altura / 2.0f, yTopo = altura / 2.0f;

    for (int i = 0; i < segmentos; i++) {
        float t0 = (float)i / segmentos, t1 = (float)(i + 1) / segmentos;
        float a0 = t0 * 2.0f * (float)M_PI, a1 = t1 * 2.0f * (float)M_PI;
        float x0 = raio * cosf(a0), z0 = raio * sinf(a0);
        float x1 = raio * cosf(a1), z1 = raio * sinf(a1);

        v.insert(v.end(), { x0,yBase,z0, t0,0.0f,  x1,yBase,z1, t1,0.0f,  x1,yTopo,z1, t1,1.0f });
        v.insert(v.end(), { x0,yBase,z0, t0,0.0f,  x1,yTopo,z1, t1,1.0f,  x0,yTopo,z0, t0,1.0f });

        v.insert(v.end(), { 0.0f,yBase,0.0f, 0.5f,0.5f,
                             x1,yBase,z1, 0.5f+0.5f*cosf(a1), 0.5f+0.5f*sinf(a1),
                             x0,yBase,z0, 0.5f+0.5f*cosf(a0), 0.5f+0.5f*sinf(a0) });

        v.insert(v.end(), { 0.0f,yTopo,0.0f, 0.5f,0.5f,
                             x0,yTopo,z0, 0.5f+0.5f*cosf(a0), 0.5f+0.5f*sinf(a0),
                             x1,yTopo,z1, 0.5f+0.5f*cosf(a1), 0.5f+0.5f*sinf(a1) });
    }
    return criaVAODeBuffer(v, nVertices);
}

GLuint criaVAOPaMoinho(int& nVertices, float comprimento, float largura, float espessura) {
    float hc = comprimento / 2.0f, hl = largura / 2.0f, he = espessura / 2.0f;

    std::vector<float> v  = verticesCubo(hc, hl, he, glm::vec3(0.0f)); // tábua horizontal
    std::vector<float> v2 = verticesCubo(hl, hc, he, glm::vec3(0.0f)); // tábua vertical
    v.insert(v.end(), v2.begin(), v2.end());

    return criaVAODeBuffer(v, nVertices);
}