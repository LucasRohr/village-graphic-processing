#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

// Carrega um arquivo de imagem do disco (jpg/png) e devolve o ID da textura já configurada na GPU
GLuint carregaTextura(const std::string& filePath);

struct TexturasVila {
    GLuint madeira;
    GLuint telhado;
    GLuint grama;
    GLuint tronco;
    GLuint folhas;
    GLuint feno;
    GLuint nuvem;
};

// Carrega todas as texturas de assets/tex/ de uma vez e devolve os IDs já prontos
TexturasVila carregaTexturasVila();

#endif
