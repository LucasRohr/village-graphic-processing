#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

GLuint carregaTextura(const std::string& filePath) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    GLuint textureId = 0;
    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 4) format = GL_RGBA;

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Falha ao carregar textura: " << filePath << std::endl;
    }
    stbi_image_free(data);
    return textureId;
}

TexturasVila carregaTexturasVila() {
    TexturasVila tex;
    tex.madeira = carregaTextura("../assets/tex/madeira.jpg");
    tex.telhado = carregaTextura("../assets/tex/telhado.jpg");
    tex.grama   = carregaTextura("../assets/tex/grama.jpg");
    tex.tronco  = carregaTextura("../assets/tex/tronco.jpg");
    tex.folhas  = carregaTextura("../assets/tex/folhas.jpg");
    tex.feno    = carregaTextura("../assets/tex/feno.jpg");
    tex.nuvem   = carregaTextura("../assets/tex/nuvem.jpg");
    return tex;
}