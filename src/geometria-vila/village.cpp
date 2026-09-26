#include "village.h"
#include "primitives.h"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 calculaModelBase(const ObjetoCena& obj) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, obj.posicao);
    model = glm::rotate(model, glm::radians(obj.rotacaoY), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(obj.escala));
    return model;
}

static void adiciona(std::vector<ObjetoCena>& objetos, GLuint vao, int nVertices, GLuint textureId,
                      glm::vec3 posicao, float escala, float rotacaoY, TipoObjeto tipo) {
    ObjetoCena obj;
    obj.vao = vao;
    obj.nVertices = nVertices;
    obj.textureId = textureId;
    obj.posicao = posicao;
    obj.escala = escala;
    obj.rotacaoY = rotacaoY;
    obj.tipo = tipo;
    objetos.push_back(obj);
}

namespace dim {
    const float CASA_CORPO_ALT = 1.4f, CASA_TELHADO_ALT = 1.0f;
    const float CELEIRO_CORPO_ALT = 2.0f, CELEIRO_TELHADO_ALT = 1.3f;
    const float TORRE_ALT = 3.5f, TELHADO_MOINHO_ALT = 0.8f;
    const float TRONCO_ALT = 1.0f, COPA_ALT = 1.3f;
    const float FARDO_ALT = 0.4f;
}

static void adicionaMoinho(std::vector<ObjetoCena>& objetos, GLuint vaoTorre, int nTorre,
                            GLuint vaoTelhado, int nTelhado, GLuint vaoPa, int nPa,
                            const TexturasVila& tex, glm::vec3 pos) {
    float centro = dim::TORRE_ALT / 2.0f;
    float topo = dim::TORRE_ALT; // topo real do cilindro, não a metade

    adiciona(objetos, vaoTorre, nTorre, tex.madeira, pos + glm::vec3(0.0f, centro, 0.0f), 1.0f, 0.0f, MOINHO);
    adiciona(objetos, vaoTelhado, nTelhado, tex.telhado,
             pos + glm::vec3(0.0f, topo + dim::TELHADO_MOINHO_ALT / 2.0f, 0.0f), 1.0f, 0.0f, MOINHO);

    // Pá logo abaixo do topo da torre, na frente do telhado
    adiciona(objetos, vaoPa, nPa, tex.tronco, pos + glm::vec3(0.0f, topo - 0.3f, 0.4f), 1.0f, 0.0f, MOINHO_PA);
}

static void adicionaCasa(std::vector<ObjetoCena>& objetos, GLuint vaoCorpo, int nCorpo,
                          GLuint vaoTelhado, int nTelhado, const TexturasVila& tex,
                          glm::vec3 pos, float rotacaoY) {
    float centro = dim::CASA_CORPO_ALT / 2.0f;
    float topo = dim::CASA_CORPO_ALT; // topo real do corpo, não a metade
    adiciona(objetos, vaoCorpo, nCorpo, tex.madeira, pos + glm::vec3(0.0f, centro, 0.0f), 1.0f, rotacaoY, CASA);
    adiciona(objetos, vaoTelhado, nTelhado, tex.telhado,
             pos + glm::vec3(0.0f, topo + dim::CASA_TELHADO_ALT / 2.0f, 0.0f), 1.0f, rotacaoY, CASA);
}

static void adicionaCeleiro(std::vector<ObjetoCena>& objetos, GLuint vaoCorpo, int nCorpo,
                             GLuint vaoTelhado, int nTelhado, const TexturasVila& tex,
                             glm::vec3 pos, float rotacaoY) {
    float centro = dim::CELEIRO_CORPO_ALT / 2.0f;
    float topo = dim::CELEIRO_CORPO_ALT;
    adiciona(objetos, vaoCorpo, nCorpo, tex.madeira, pos + glm::vec3(0.0f, centro, 0.0f), 1.0f, rotacaoY, CELEIRO);
    adiciona(objetos, vaoTelhado, nTelhado, tex.telhado,
             pos + glm::vec3(0.0f, topo + dim::CELEIRO_TELHADO_ALT / 2.0f, 0.0f), 1.0f, rotacaoY, CELEIRO);
}

static void adicionaArvore(std::vector<ObjetoCena>& objetos, GLuint vaoTronco, int nTronco,
                            GLuint vaoCopa, int nCopa, const TexturasVila& tex, glm::vec3 pos) {
    float centro = dim::TRONCO_ALT / 2.0f;
    float topo = dim::TRONCO_ALT;
    adiciona(objetos, vaoTronco, nTronco, tex.tronco, pos + glm::vec3(0.0f, centro, 0.0f), 1.0f, 0.0f, ARVORE);
    adiciona(objetos, vaoCopa, nCopa, tex.folhas,
             pos + glm::vec3(0.0f, topo + dim::COPA_ALT / 2.0f, 0.0f), 1.0f, 0.0f, ARVORE);
}

static void adicionaFardo(std::vector<ObjetoCena>& objetos, GLuint vaoFardo, int nFardo,
                           const TexturasVila& tex, glm::vec3 pos) {
    adiciona(objetos, vaoFardo, nFardo, tex.feno, pos + glm::vec3(0.0f, dim::FARDO_ALT / 2.0f, 0.0f), 1.0f, 0.0f, FARDO);
}

static void adicionaNuvem(std::vector<ObjetoCena>& objetos, GLuint vaoCubo, int nVertCubo,
                           const TexturasVila& tex, glm::vec3 pos) {
    adiciona(objetos, vaoCubo, nVertCubo, tex.nuvem, pos, 1.5f, 0.0f, NUVEM);
}

void geraVila(std::vector<ObjetoCena>& objetos, const TexturasVila& tex) {
    int n;

    GLuint vaoTerreno = criaVAOPlano(n, 40.0f, 40.0f); int nTerreno = n;

    GLuint vaoCasaCorpo = criaVAOCubo(n, 1.4f, dim::CASA_CORPO_ALT, 1.4f); int nCasaCorpo = n;
    GLuint vaoCasaTelhado = criaVAOPiramide(n, 1.9f, dim::CASA_TELHADO_ALT); int nCasaTelhado = n;

    GLuint vaoCeleiroCorpo = criaVAOCubo(n, 2.2f, dim::CELEIRO_CORPO_ALT, 2.4f); int nCeleiroCorpo = n;
    GLuint vaoCeleiroTelhado = criaVAOPiramide(n, 2.6f, dim::CELEIRO_TELHADO_ALT); int nCeleiroTelhado = n;

    GLuint vaoTorre = criaVAOCilindro(n, 20, 0.35f, dim::TORRE_ALT); int nTorre = n;
    GLuint vaoTelhadoMoinho = criaVAOPiramide(n, 0.9f, dim::TELHADO_MOINHO_ALT); int nTelhadoMoinho = n;
    GLuint vaoPa = criaVAOPaMoinho(n); int nPa = n;

    GLuint vaoTronco = criaVAOCilindro(n, 10, 0.12f, dim::TRONCO_ALT); int nTronco = n;
    GLuint vaoCopa = criaVAOPiramide(n, 1.0f, dim::COPA_ALT); int nCopa = n;

    GLuint vaoFardo = criaVAOCilindro(n, 16, 0.5f, dim::FARDO_ALT); int nFardo = n;
    GLuint vaoNuvem = criaVAOCubo(n, 1.0f, 1.0f, 1.0f); int nNuvem = n;

    adiciona(objetos, vaoTerreno, nTerreno, tex.grama, glm::vec3(0.0f), 1.0f, 0.0f, TERRENO);

    adicionaCasa(objetos, vaoCasaCorpo, nCasaCorpo, vaoCasaTelhado, nCasaTelhado, tex, glm::vec3(-8.0f, 0.0f, -6.0f), 0.0f);
    adicionaCasa(objetos, vaoCasaCorpo, nCasaCorpo, vaoCasaTelhado, nCasaTelhado, tex, glm::vec3(-4.0f, 0.0f, -6.0f), 20.0f);
    adicionaCasa(objetos, vaoCasaCorpo, nCasaCorpo, vaoCasaTelhado, nCasaTelhado, tex, glm::vec3(0.0f, 0.0f, -7.0f), -15.0f);

    adicionaCeleiro(objetos, vaoCeleiroCorpo, nCeleiroCorpo, vaoCeleiroTelhado, nCeleiroTelhado, tex, glm::vec3(8.0f, 0.0f, -4.0f), 0.0f);
    adicionaCeleiro(objetos, vaoCeleiroCorpo, nCeleiroCorpo, vaoCeleiroTelhado, nCeleiroTelhado, tex, glm::vec3(10.0f, 0.0f, 2.0f), 90.0f);

    // Afastados o suficiente das paredes dos celeiros (que têm ~1.1-1.2 de meia-largura)
    adicionaFardo(objetos, vaoFardo, nFardo, tex, glm::vec3(5.7f, 0.0f, -4.6f));
    adicionaFardo(objetos, vaoFardo, nFardo, tex, glm::vec3(5.7f, 0.0f, -3.4f));
    adicionaFardo(objetos, vaoFardo, nFardo, tex, glm::vec3(5.0f, 0.0f, -4.0f));
    adicionaFardo(objetos, vaoFardo, nFardo, tex, glm::vec3(12.6f, 0.0f, 1.4f));
    adicionaFardo(objetos, vaoFardo, nFardo, tex, glm::vec3(12.6f, 0.0f, 2.6f));

    adicionaMoinho(objetos, vaoTorre, nTorre, vaoTelhadoMoinho, nTelhadoMoinho, vaoPa, nPa, tex, glm::vec3(-10.0f, 0.0f, 4.0f));
    adicionaMoinho(objetos, vaoTorre, nTorre, vaoTelhadoMoinho, nTelhadoMoinho, vaoPa, nPa, tex, glm::vec3(4.0f, 0.0f, 8.0f));

    glm::vec3 posArvores[6] = {
        {-14.0f, 0.0f, 0.0f}, {-2.0f, 0.0f, 6.0f}, {2.0f, 0.0f, -10.0f},
        {12.0f, 0.0f, -8.0f}, {-6.0f, 0.0f, 8.0f}, {14.0f, 0.0f, 6.0f}
    };
    for (auto& p : posArvores)
        adicionaArvore(objetos, vaoTronco, nTronco, vaoCopa, nCopa, tex, p);

    glm::vec3 posNuvens[5] = {
        {-10.0f, 8.0f, 0.0f}, {-4.0f, 9.0f, -4.0f}, {2.0f, 8.5f, 3.0f},
        {8.0f, 9.5f, -2.0f}, {-6.0f, 8.0f, 5.0f}
    };
    for (auto& p : posNuvens)
        adicionaNuvem(objetos, vaoNuvem, nNuvem, tex, p);
}