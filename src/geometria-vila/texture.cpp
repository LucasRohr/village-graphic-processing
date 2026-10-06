#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION // só pode aparecer uma vez no projeto inteiro; gera a implementação da biblioteca stb_image aqui
#include "stb_image.h"
#include <iostream>

GLuint carregaTextura(const std::string& filePath) {
    // O OpenGL espera a origem (0,0) da textura no canto INFERIOR esquerdo, mas
    // a maioria dos formatos de imagem guarda o primeiro pixel no canto
    // superior; essa flag inverte o eixo Y na hora de ler, evitando texturas de cabeça para baixo
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    // stbi_load decodifica o arquivo e devolve um ponteiro para os pixels em
    // memória; nrChannels é descoberto automaticamente (1 = cinza, 3 = RGB, 4 = RGBA)
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    GLuint textureId = 0;
    if (data) {
        // Escolhe o formato de cor do OpenGL de acordo com o número de canais da imagem carregada
        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 4) format = GL_RGBA;

        glGenTextures(1, &textureId);           // reserva 1 identificador de textura
        glBindTexture(GL_TEXTURE_2D, textureId); // ativa essa textura

        // GL_REPEAT: se a coordenada UV passar de 1.0 ou ficar negativa, a
        // textura se repete (útil para madeira/grama cobrindo áreas maiores)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Filtro de minificação com mipmap (textura vista de longe, menor que o
        // original) interpola entre os dois níveis de mipmap mais próximos, suavizando o resultado
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // Filtro de magnificação (textura vista de perto, maior que o original) interpola os pixels vizinhos
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Envia os pixels da RAM para a VRAM
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // gera automaticamente as versões reduzidas (mipmaps) usadas pelo filtro acima
    } else {
        std::cerr << "Falha ao carregar textura: " << filePath << std::endl;
    }
    stbi_image_free(data); // libera o buffer em RAM pois a GPU já tem sua própria cópia dos pixels
    return textureId;
}

TexturasVila carregaTexturasVila() {
    // Um carregaTextura() por arquivo
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
