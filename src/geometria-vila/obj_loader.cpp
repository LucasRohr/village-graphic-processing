#include "obj_loader.h"
#include "texture.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

// Idêntica à função de primitives.cpp (duplicada aqui para este arquivo não depender do outro)
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

GLuint carregaOBJ(const std::string& filePath, int& nVertices) {
    // Buffers temporários com os dados "crus" do arquivo, na ordem em que aparecem
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<float> vBuffer; // buffer final, já no layout intercalado (posição+UV) que vai para a GPU

    std::ifstream arquivo(filePath);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo .obj: " << filePath << std::endl;
        nVertices = 0;
        return 0;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::istringstream ss(linha);
        std::string tipo;
        ss >> tipo; // primeira palavra da linha identifica o tipo de dado ("v", "vt", "f", etc.)

        if (tipo == "v") {
            // Linha de posição: "v x y z"
            glm::vec3 v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (tipo == "vt") {
            // Linha de coordenada de textura: "vt u v"
            glm::vec2 vt;
            ss >> vt.x >> vt.y;
            texCoords.push_back(vt);
        } else if (tipo == "f") {
            // Linha de face: "f v1/vt1 v2/vt2 v3/vt3" (índices baseados em 1)
            std::string token;
            while (ss >> token) { // percorre cada "v/vt" da face (3 tokens, já que a face é triangular)
                int vi = 0, ti = -1;
                std::istringstream tokenStream(token);
                std::string parte;

                // Separa o token pelo delimitador '/': primeira parte é o índice de posição, segunda é o índice de UV
                if (std::getline(tokenStream, parte, '/'))
                    vi = !parte.empty() ? std::stoi(parte) - 1 : 0; // -1 porque o .obj indexa a partir de 1, e nossos vetores a partir de 0
                if (std::getline(tokenStream, parte, '/'))
                    ti = !parte.empty() ? std::stoi(parte) - 1 : -1;

                // Empacota a posição deste vértice da face no buffer final
                vBuffer.push_back(vertices[vi].x);
                vBuffer.push_back(vertices[vi].y);
                vBuffer.push_back(vertices[vi].z);
                // Empacota a UV, se a face tiver referenciado uma, senão usa (0,0) como fallback
                if (ti >= 0 && ti < (int)texCoords.size()) {
                    vBuffer.push_back(texCoords[ti].x);
                    vBuffer.push_back(texCoords[ti].y);
                } else {
                    vBuffer.push_back(0.0f);
                    vBuffer.push_back(0.0f);
                }
            }
        }
        // Outras linhas do .obj (comentários "#", normais "vn", material "usemtl" etc.) não são reconhecidas por nenhum "if" acima e são simplesmente ignoradas
    }

    return criaVAODeBuffer(vBuffer, nVertices);
}

void carregaAnimais(std::vector<ObjetoCena>& objetos) {
    // Dados de cada instância de animal: de qual arquivo .obj carregar, onde
    // posicionar, em que escala e com qual rotação em Y
    struct AnimalInfo {
        std::string objPath;
        glm::vec3 posicao;
        float escala;
        float rotacaoY;
    };

    // Todos os animais do pacote Cube Pets compartilham a mesma folha de
    // textura (colormap.png), então ela é carregada uma única vez fora do loop
    GLuint texturaAnimais = carregaTextura("../assets/Modelos3D/AnimaisFazenda/colormap.png");

    // 3 animais: 2 vacas lado a lado (perto do celeiro 1) e 1 porco mais afastado (perto do celeiro 2)
    // escala 0.6 reduz o modelo original para um tamanho condizente com o resto da vila
    std::vector<AnimalInfo> animais = {
        { "../assets/Modelos3D/AnimaisFazenda/animal-cow.obj",   glm::vec3(7.0f, 0.0f, -2.0f), 0.6f, 0.0f },
        { "../assets/Modelos3D/AnimaisFazenda/animal-cow.obj",   glm::vec3(8.0f, 0.0f, -2.0f), 0.6f, 0.0f },
        { "../assets/Modelos3D/AnimaisFazenda/animal-pig.obj",   glm::vec3(9.5f, 0.0f, 5.0f), 0.6f, 0.0f },
    };

    for (auto& a : animais) {
        int nVertices;
        GLuint vao = carregaOBJ(a.objPath, nVertices); // um VAO por animal (modelos diferentes = geometria diferente)

        // Monta o ObjetoCena manualmente
        ObjetoCena obj;
        obj.vao = vao;
        obj.nVertices = nVertices;
        obj.textureId = texturaAnimais;
        obj.posicao = a.posicao;
        obj.escala = a.escala;
        obj.rotacaoY = a.rotacaoY;
        obj.tipo = ANIMAL;
        objetos.push_back(obj);
    }
}
