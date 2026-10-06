#include "village.h"
#include "primitives.h"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 calculaModelBase(const ObjetoCena& obj) {
    glm::mat4 model = glm::mat4(1.0f); // começa da matriz identidade (sem nenhuma transformação)
    // Ordem importa: como as matrizes multiplicam da direita para a esquerda
    // sobre o vértice, isso aplica primeiro a escala, depois a rotação, depois
    // a translação (ou seja: objeto nasce na origem, já na escala e orientação
    // corretas, e só então é "teleportado" para sua posição final no mundo)
    model = glm::translate(model, obj.posicao);
    model = glm::rotate(model, glm::radians(obj.rotacaoY), glm::vec3(0.0f, 1.0f, 0.0f)); // gira em torno do eixo Y (vertical)
    model = glm::scale(model, glm::vec3(obj.escala)); // mesma escala nos 3 eixos (uniforme)
    return model;
}

// Monta um ObjetoCena com os dados recebidos e o adiciona ao vetor da cena
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

// Alturas usadas tanto para gerar a geometria de cada peça quanto para empilhar corpo+telhado corretamente
namespace dim {
    const float CASA_CORPO_ALT = 1.4f, CASA_TELHADO_ALT = 1.0f;
    const float CELEIRO_CORPO_ALT = 2.0f, CELEIRO_TELHADO_ALT = 1.3f;
    const float TORRE_ALT = 3.5f, TELHADO_MOINHO_ALT = 0.8f;
    const float TRONCO_ALT = 1.0f, COPA_ALT = 1.3f;
    const float FARDO_ALT = 0.4f;
}

// Monta um moinho a partir de 3 peças já geradas (torre, telhado, pá),
// posicionando cada uma em relação a base
static void adicionaMoinho(std::vector<ObjetoCena>& objetos, GLuint vaoTorre, int nTorre,
                            GLuint vaoTelhado, int nTelhado, GLuint vaoPa, int nPa,
                            const TexturasVila& tex, glm::vec3 pos) {
    float centro = dim::TORRE_ALT / 2.0f;
    float topo = dim::TORRE_ALT;

    // Cria a torre do moinho
    adiciona(objetos, vaoTorre, nTorre, tex.madeira, pos + glm::vec3(0.0f, centro, 0.0f), 1.0f, 0.0f, MOINHO);
    // Cria o telhado apoiado em cima da torre
    adiciona(objetos, vaoTelhado, nTelhado, tex.telhado,
             pos + glm::vec3(0.0f, topo + dim::TELHADO_MOINHO_ALT / 2.0f, 0.0f), 1.0f, 0.0f, MOINHO);

    // Cria pá posicionada um pouco abaixo do topo
    adiciona(objetos, vaoPa, nPa, tex.tronco, pos + glm::vec3(0.0f, topo - 0.3f, 0.4f), 1.0f, 0.0f, MOINHO_PA);
}

// Monta uma casa (corpo + telhado), podendo ser rotacionada em Y para variar a orientação
static void adicionaCasa(std::vector<ObjetoCena>& objetos, GLuint vaoCorpo, int nCorpo,
                          GLuint vaoTelhado, int nTelhado, const TexturasVila& tex,
                          glm::vec3 pos, float rotacaoY) {
    float centro = dim::CASA_CORPO_ALT / 2.0f;
    float topo = dim::CASA_CORPO_ALT;
    adiciona(objetos, vaoCorpo, nCorpo, tex.madeira, pos + glm::vec3(0.0f, centro, 0.0f), 1.0f, rotacaoY, CASA);
    adiciona(objetos, vaoTelhado, nTelhado, tex.telhado,
             pos + glm::vec3(0.0f, topo + dim::CASA_TELHADO_ALT / 2.0f, 0.0f), 1.0f, rotacaoY, CASA);
}

// Mesma lógica da casa, só com dimensões maiores (ver dim)
static void adicionaCeleiro(std::vector<ObjetoCena>& objetos, GLuint vaoCorpo, int nCorpo,
                             GLuint vaoTelhado, int nTelhado, const TexturasVila& tex,
                             glm::vec3 pos, float rotacaoY) {
    float centro = dim::CELEIRO_CORPO_ALT / 2.0f;
    float topo = dim::CELEIRO_CORPO_ALT;
    adiciona(objetos, vaoCorpo, nCorpo, tex.madeira, pos + glm::vec3(0.0f, centro, 0.0f), 1.0f, rotacaoY, CELEIRO);
    adiciona(objetos, vaoTelhado, nTelhado, tex.telhado,
             pos + glm::vec3(0.0f, topo + dim::CELEIRO_TELHADO_ALT / 2.0f, 0.0f), 1.0f, rotacaoY, CELEIRO);
}

// Árvore: tronco (cilindro fino) + copa (pirâmide), empilhados do mesmo jeito que corpo+telhado das construções
static void adicionaArvore(std::vector<ObjetoCena>& objetos, GLuint vaoTronco, int nTronco,
                            GLuint vaoCopa, int nCopa, const TexturasVila& tex, glm::vec3 pos) {
    float centro = dim::TRONCO_ALT / 2.0f;
    float topo = dim::TRONCO_ALT;
    adiciona(objetos, vaoTronco, nTronco, tex.tronco, pos + glm::vec3(0.0f, centro, 0.0f), 1.0f, 0.0f, ARVORE);
    adiciona(objetos, vaoCopa, nCopa, tex.folhas,
             pos + glm::vec3(0.0f, topo + dim::COPA_ALT / 2.0f, 0.0f), 1.0f, 0.0f, ARVORE);
}

// Fardo de feno: cilindro "deitado" visualmente, já que é baixo e largo
static void adicionaFardo(std::vector<ObjetoCena>& objetos, GLuint vaoFardo, int nFardo,
                           const TexturasVila& tex, glm::vec3 pos) {
    adiciona(objetos, vaoFardo, nFardo, tex.feno, pos + glm::vec3(0.0f, dim::FARDO_ALT / 2.0f, 0.0f), 1.0f, 0.0f, FARDO);
}

// Nuvem: um cubo escalado para 1.5x, sem ajuste de altura (sua posição Y já
// vem pronta da lista de posições em geraVila, lá no céu)
static void adicionaNuvem(std::vector<ObjetoCena>& objetos, GLuint vaoCubo, int nVertCubo,
                           const TexturasVila& tex, glm::vec3 pos) {
    adiciona(objetos, vaoCubo, nVertCubo, tex.nuvem, pos, 1.5f, 0.0f, NUVEM);
}

void geraVila(std::vector<ObjetoCena>& objetos, const TexturasVila& tex) {
    int n; // variável temporária reaproveitada para receber nVertices de cada criaVAO*

    // Gera UMA vez o VAO de cada peça (terreno, corpo de casa, telhado, etc.);
    // as instâncias abaixo reaproveitam o mesmo VAO/nVertices em posições diferentes
    GLuint vaoTerreno = criaVAOPlano(n, 40.0f, 40.0f); int nTerreno = n; // chão de 40x40 unidades

    GLuint vaoCasaCorpo = criaVAOCubo(n, 1.4f, dim::CASA_CORPO_ALT, 1.4f); int nCasaCorpo = n;
    GLuint vaoCasaTelhado = criaVAOPiramide(n, 1.9f, dim::CASA_TELHADO_ALT); int nCasaTelhado = n; // base do telhado (1.9) maior que o corpo (1.4) pra sobrar beiral

    GLuint vaoCeleiroCorpo = criaVAOCubo(n, 2.2f, dim::CELEIRO_CORPO_ALT, 2.4f); int nCeleiroCorpo = n;
    GLuint vaoCeleiroTelhado = criaVAOPiramide(n, 2.6f, dim::CELEIRO_TELHADO_ALT); int nCeleiroTelhado = n;

    GLuint vaoTorre = criaVAOCilindro(n, 20, 0.35f, dim::TORRE_ALT); int nTorre = n; // 20 lados = cilindro bem arredondado, raio fino (0.35) pra parecer torre
    GLuint vaoTelhadoMoinho = criaVAOPiramide(n, 0.9f, dim::TELHADO_MOINHO_ALT); int nTelhadoMoinho = n;
    GLuint vaoPa = criaVAOPaMoinho(n); int nPa = n; // usa os valores default de primitives.h (comprimento 1.6, largura 0.15, espessura 0.06)

    GLuint vaoTronco = criaVAOCilindro(n, 10, 0.12f, dim::TRONCO_ALT); int nTronco = n; // só 10 lados (tronco é fino, não precisa de tanto detalhe)
    GLuint vaoCopa = criaVAOPiramide(n, 1.0f, dim::COPA_ALT); int nCopa = n;

    GLuint vaoFardo = criaVAOCilindro(n, 16, 0.5f, dim::FARDO_ALT); int nFardo = n;
    GLuint vaoNuvem = criaVAOCubo(n, 1.0f, 1.0f, 1.0f); int nNuvem = n;

    // Terreno de base para as demais contruções, com textura de grama
    adiciona(objetos, vaoTerreno, nTerreno, tex.grama, glm::vec3(0.0f), 1.0f, 0.0f, TERRENO);

    // 3 casas espalhadas, cada uma com uma rotação Y diferente (0°, 20°, -15°)
    //  pra variar a orientação e a cena não ficar com tudo alinhado na mesma direção
    adicionaCasa(objetos, vaoCasaCorpo, nCasaCorpo, vaoCasaTelhado, nCasaTelhado, tex, glm::vec3(-8.0f, 0.0f, -6.0f), 0.0f);
    adicionaCasa(objetos, vaoCasaCorpo, nCasaCorpo, vaoCasaTelhado, nCasaTelhado, tex, glm::vec3(-4.0f, 0.0f, -6.0f), 20.0f);
    adicionaCasa(objetos, vaoCasaCorpo, nCasaCorpo, vaoCasaTelhado, nCasaTelhado, tex, glm::vec3(0.0f, 0.0f, -7.0f), -15.0f);

    // 2 celeiros
    adicionaCeleiro(objetos, vaoCeleiroCorpo, nCeleiroCorpo, vaoCeleiroTelhado, nCeleiroTelhado, tex, glm::vec3(8.0f, 0.0f, -4.0f), 0.0f);
    adicionaCeleiro(objetos, vaoCeleiroCorpo, nCeleiroCorpo, vaoCeleiroTelhado, nCeleiroTelhado, tex, glm::vec3(10.0f, 0.0f, 2.0f), 90.0f);

    // 5 fardos de feno espalhados
    adicionaFardo(objetos, vaoFardo, nFardo, tex, glm::vec3(5.7f, 0.0f, -4.6f));
    adicionaFardo(objetos, vaoFardo, nFardo, tex, glm::vec3(5.7f, 0.0f, -3.4f));
    adicionaFardo(objetos, vaoFardo, nFardo, tex, glm::vec3(5.0f, 0.0f, -4.0f));
    adicionaFardo(objetos, vaoFardo, nFardo, tex, glm::vec3(12.6f, 0.0f, 1.4f));
    adicionaFardo(objetos, vaoFardo, nFardo, tex, glm::vec3(12.6f, 0.0f, 2.6f));

    // 2 moinhos espalhados pela vila
    adicionaMoinho(objetos, vaoTorre, nTorre, vaoTelhadoMoinho, nTelhadoMoinho, vaoPa, nPa, tex, glm::vec3(-10.0f, 0.0f, 4.0f));
    adicionaMoinho(objetos, vaoTorre, nTorre, vaoTelhadoMoinho, nTelhadoMoinho, vaoPa, nPa, tex, glm::vec3(4.0f, 0.0f, 8.0f));

    // 6 árvores posicionadas distribuidas pela vila sem invadir onde ficam as construções
    glm::vec3 posArvores[6] = {
        {-14.0f, 0.0f, 0.0f}, {-2.0f, 0.0f, 6.0f}, {2.0f, 0.0f, -10.0f},
        {12.0f, 0.0f, -8.0f}, {-6.0f, 0.0f, 8.0f}, {14.0f, 0.0f, 6.0f}
    };
    for (auto& p : posArvores)
        adicionaArvore(objetos, vaoTronco, nTronco, vaoCopa, nCopa, tex, p);

    // 5 nuvens, todas com Y alto (8 a 9.5, bem acima das construções)
    // a posição X de cada uma vira o ponto de partida da animação de deslocamento contínuo
    glm::vec3 posNuvens[5] = {
        {-10.0f, 8.0f, 0.0f}, {-4.0f, 9.0f, -4.0f}, {2.0f, 8.5f, 3.0f},
        {8.0f, 9.5f, -2.0f}, {-6.0f, 8.0f, 5.0f}
    };
    for (auto& p : posNuvens)
        adicionaNuvem(objetos, vaoNuvem, nNuvem, tex, p);
}
