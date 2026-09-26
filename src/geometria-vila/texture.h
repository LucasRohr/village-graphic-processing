#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

GLuint carregaTextura(const std::string& filePath);

// IDs de todas as texturas da vila
struct TexturasVila {
    GLuint madeira; // paredes de casa/celeiro/moinho
    GLuint telhado; // telhados e pá do moinho
    GLuint grama;   // terreno
    GLuint tronco;  // tronco da árvore
    GLuint folhas;  // copa da árvore
    GLuint feno;    // fardos
    GLuint nuvem;   // nuvens
};

TexturasVila carregaTexturasVila();

#endif