#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

// Carrega um arquivo de imagem do disco (jpg/png) e devolve o ID da textura já
// configurada na GPU. Retorna 0 em caso de falha
GLuint carregaTextura(const std::string& filePath);

// IDs de todas as texturas da vila (uma por "material"; cada objeto da cena
// aponta para um destes IDs em vez de carregar a própria textura)
struct TexturasVila {
    GLuint madeira; // paredes de casa/celeiro/moinho
    GLuint telhado; // telhados e pá do moinho
    GLuint grama;   // terreno
    GLuint tronco;  // tronco da árvore
    GLuint folhas;  // copa da árvore
    GLuint feno;    // fardos
    GLuint nuvem;   // nuvens
};

// Carrega todas as texturas de assets/tex/ de uma vez e devolve os IDs já prontos
TexturasVila carregaTexturasVila();

#endif
