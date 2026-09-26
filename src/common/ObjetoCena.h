#ifndef OBJETO_CENA_H
#define OBJETO_CENA_H

#include <glad/glad.h>
#include <glm/glm.hpp>

// Tipo de cada elemento da vila; usado tanto para instanciar quanto para decidir
// comportamento especial (quais tipos ficam rosa com a tecla R, quais animam, etc.)
enum TipoObjeto {
    TERRENO,
    CASA,
    CELEIRO,
    MOINHO,
    MOINHO_PA,
    ARVORE,
    FARDO,
    NUVEM,
    ANIMAL
};

// Um objeto instanciado na cena: geometria (VAO) + textura + transformação.
// É a struct que conecta a frente de geometria (preenche o vetor de objetos)
// com a frente de câmera (usa os dados para desenhar a cena e o minimapa)
struct ObjetoCena {
    GLuint vao;
    int nVertices;
    GLuint textureId;
    glm::vec3 posicao;
    float escala;
    float rotacaoY;
    TipoObjeto tipo;
};

#endif
