#include "obj_loader.h"
#include "texture.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

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
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<float> vBuffer;

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
        ss >> tipo;

        if (tipo == "v") {
            glm::vec3 v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (tipo == "vt") {
            glm::vec2 vt;
            ss >> vt.x >> vt.y;
            texCoords.push_back(vt);
        } else if (tipo == "f") {
            std::vector<glm::ivec2> face; // x = indice posicao, y = indice textura

            std::string token;
            while (ss >> token) {
                int vi = 0, ti = -1;
                std::istringstream tokenStream(token);
                std::string parte;

                if (std::getline(tokenStream, parte, '/'))
                    vi = !parte.empty() ? std::stoi(parte) - 1 : 0;
                if (std::getline(tokenStream, parte, '/'))
                    ti = !parte.empty() ? std::stoi(parte) - 1 : -1;
                // terceiro campo (normal) é ignorado -- pipeline atual não faz iluminação

                face.push_back(glm::ivec2(vi, ti));
            }

            // Triangulação em leque: funciona pra faces com 3, 4 ou mais vértices
            for (size_t i = 1; i + 1 < face.size(); i++) {
                glm::ivec2 tri[3] = { face[0], face[i], face[i + 1] };
                for (int j = 0; j < 3; j++) {
                    int vi = tri[j].x, ti = tri[j].y;
                    vBuffer.push_back(vertices[vi].x);
                    vBuffer.push_back(vertices[vi].y);
                    vBuffer.push_back(vertices[vi].z);
                    if (ti >= 0 && ti < (int)texCoords.size()) {
                        vBuffer.push_back(texCoords[ti].x);
                        vBuffer.push_back(texCoords[ti].y);
                    } else {
                        vBuffer.push_back(0.0f);
                        vBuffer.push_back(0.0f);
                    }
                }
            }
        }
    }

    return criaVAODeBuffer(vBuffer, nVertices);
}

void carregaAnimais(std::vector<ObjetoCena>& objetos) {
    struct AnimalInfo {
        std::string objPath;
        glm::vec3 posicao;
        float escala;
        float rotacaoY;
    };

    GLuint texturaAnimais = carregaTextura("../assets/Modelos3D/AnimaisFazenda/colormap.png");

    std::vector<AnimalInfo> animais = {
        { "../assets/Modelos3D/AnimaisFazenda/animal-cow.obj",   glm::vec3(7.0f, 0.0f, -2.0f), 0.6f, 0.0f },
        { "../assets/Modelos3D/AnimaisFazenda/animal-sheep.obj", glm::vec3(-6.0f, 0.0f, 2.0f), 0.6f, 0.0f },
        { "../assets/Modelos3D/AnimaisFazenda/animal-pig.obj",   glm::vec3(9.5f, 0.0f, 2.0f), 0.6f, 0.0f },
    };

    for (auto& a : animais) {
        int nVertices;
        GLuint vao = carregaOBJ(a.objPath, nVertices);

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