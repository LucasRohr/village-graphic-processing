#include "primitives.h"
#include <vector>
#include <cmath>
#include <glm/glm.hpp>

// Recebe um buffer já pronto (posição XYZ + UV intercalados) e cria o VAO/VBO correspondentes na GPU. 
// Todas as funções de criaVAO* abaixo delegam para esta, então o layout de atributos é o mesmo
// em toda a vila (location 0 = posição, location 1 = UV)
static GLuint criaVAODeBuffer(const std::vector<float>& dados, int& nVertices) {
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao); // reserva 1 identificador de VAO
    glBindVertexArray(vao);     // ativa esse VAO; as chamadas seguintes de VBO/atributo ficam associadas a ele

    glGenBuffers(1, &vbo);                      // reserva 1 identificador de VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);          // ativa o VBO como buffer de atributos de vértice
    glBufferData(GL_ARRAY_BUFFER, dados.size() * sizeof(float), dados.data(), GL_STATIC_DRAW);

    // GL_STATIC_DRAW avisa que esses dados não vão mudar depois de enviados (a geometria é fixa)

    int stride = 5 * sizeof(float); // cada vértice ocupa 5 floats: 3 de posição (X,Y,Z) + 2 de UV (S,T)

    // Atributo 0 (posição)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo 1 (UV)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    nVertices = (int)(dados.size() / 5); // total de vértices = total de floats / 5 por vértice
    return vao;
}

// Gera os 36 vértices (posição+UV) de um cubo com meias-dimensões, sem criar VAO (permite combinar vários cubos num único mesh)
static std::vector<float> verticesCubo(float hx, float hy, float hz, glm::vec3 offset) {
    // hx/hy/hz são as MEIAS dimensões (metade da largura/altura/profundidade),
    // já que o cubo é centrada na origem e se estende de -h a +h em cada eixo.
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
    // Desloca todos os vértices pelo offset recebido, permitindo posicionar
    // este cubo em relação à origem do mesh
    for (size_t i = 0; i < v.size(); i += 5) {
        v[i]   += offset.x;
        v[i+1] += offset.y;
        v[i+2] += offset.z;
    }
    return v;
}

GLuint criaVAOCubo(int& nVertices, float largura, float altura, float profundidade) {
    // Converte as dimensões totais em meias-dimensões (a função verticesCubo
    // trabalha com o cubo centrado na origem, então precisa da metade de cada eixo)
    std::vector<float> v = verticesCubo(largura / 2.0f, altura / 2.0f, profundidade / 2.0f, glm::vec3(0.0f));
    return criaVAODeBuffer(v, nVertices);
}

GLuint criaVAOPlano(int& nVertices, float largura, float profundidade) {
    float hx = largura / 2.0f, hz = profundidade / 2.0f; // meia-largura e meia-profundidade
    // Um retângulo no plano XZ (Y fixo em 0)
    std::vector<float> v = {
        -hx, 0.0f,-hz, 0.0f,0.0f,  hx, 0.0f,-hz, 1.0f,0.0f,  hx, 0.0f, hz, 1.0f,1.0f,
         hx, 0.0f, hz, 1.0f,1.0f, -hx, 0.0f, hz, 0.0f,1.0f, -hx, 0.0f,-hz, 0.0f,0.0f,
    };
    return criaVAODeBuffer(v, nVertices);
}

GLuint criaVAOPiramide(int& nVertices, float baseTamanho, float altura) {
    float hb = baseTamanho / 2.0f, hy = altura / 2.0f; // meio-lado da base e meia-altura
    float bfl[3] = {-hb,-hy, hb}, bfr[3] = { hb,-hy, hb}; // bfl = base front left, bfr = base front right
    float btr[3] = { hb,-hy,-hb}, btl[3] = {-hb,-hy,-hb}; // btr = base back right, btl = base back left
    float ap[3]  = { 0.0f, hy, 0.0f}; // ap = apex (topo da pirâmide)

    // As 4 primeiras linhas são as 4 faces triangulares laterais (cada uma liga
    // uma aresta da base ao ápice, com UV 0.5,1.0 no topo para convergir a
    // textura no ápice), as 2 últimas linhas formam a base (2 triângulos)
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
    float yBase = -altura / 2.0f, yTopo = altura / 2.0f; // cilindro centrado verticalmente na origem

    // Percorre a circunferência em segmentos (ou fatias)
    for (int i = 0; i < segmentos; i++) {
        float t0 = (float)i / segmentos, t1 = (float)(i + 1) / segmentos; // posição no intervalo [0,1]
        float a0 = t0 * 2.0f * (float)M_PI, a1 = t1 * 2.0f * (float)M_PI; // mesma posição em radianos
        float x0 = raio * cosf(a0), z0 = raio * sinf(a0); // ponto na borda inferior/superior no início da fatia
        float x1 = raio * cosf(a1), z1 = raio * sinf(a1); // ponto na borda no fim da fatia

        // Lateral do cilindro: 2 triângulos formando um retângulo entre a base e o topo
        v.insert(v.end(), { x0,yBase,z0, t0,0.0f,  x1,yBase,z1, t1,0.0f,  x1,yTopo,z1, t1,1.0f });
        v.insert(v.end(), { x0,yBase,z0, t0,0.0f,  x1,yTopo,z1, t1,1.0f,  x0,yTopo,z0, t0,1.0f });

        // Tampa de baixo: triângulo ligando o centro aos dois pontos da borda desta fatia
        v.insert(v.end(), { 0.0f,yBase,0.0f, 0.5f,0.5f,
                             x1,yBase,z1, 0.5f+0.5f*cosf(a1), 0.5f+0.5f*sinf(a1),
                             x0,yBase,z0, 0.5f+0.5f*cosf(a0), 0.5f+0.5f*sinf(a0) });

        // Tampa de cima: mesma ideia da tampa de baixo
        v.insert(v.end(), { 0.0f,yTopo,0.0f, 0.5f,0.5f,
                             x0,yTopo,z0, 0.5f+0.5f*cosf(a0), 0.5f+0.5f*sinf(a0),
                             x1,yTopo,z1, 0.5f+0.5f*cosf(a1), 0.5f+0.5f*sinf(a1) });
    }
    return criaVAODeBuffer(v, nVertices);
}

GLuint criaVAOPaMoinho(int& nVertices, float comprimento, float largura, float espessura) {
    float hc = comprimento / 2.0f, hl = largura / 2.0f, he = espessura / 2.0f; // meias-dimensões das tábuas

    std::vector<float> v  = verticesCubo(hc, hl, he, glm::vec3(0.0f)); // tábua horizontal
    std::vector<float> v2 = verticesCubo(hl, hc, he, glm::vec3(0.0f)); // tábua vertical
    v.insert(v.end(), v2.begin(), v2.end()); // concatena as duas no mesmo buffer/mesh

    return criaVAODeBuffer(v, nVertices);
}
