#include "minimap.h"
#include "village.h" // calculaModelBase, já pronta na frente de geometria -- reaproveitada, não duplicada
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Tamanho (em pixels) e margem do quadrado do minimapa, no canto superior direito
static const int TAMANHO_MINIMAPA = 220;
static const int MARGEM = 15;

// Metade da extensão do mundo (em unidades) que o minimapa enquadra. A vila
// ocupa posições entre aproximadamente -14 e 14 em X/Z (ver village.cpp),
// então 20 dá uma margem boa sem sobrar espaço vazio demais
static const float ALCANCE_MUNDO = 20.0f;

void desenhaMinimapa(const std::vector<ObjetoCena>& objetos, GLuint shaderProgram, int screenWidth, int screenHeight) {
    int x = screenWidth - TAMANHO_MINIMAPA - MARGEM;
    int y = screenHeight - TAMANHO_MINIMAPA - MARGEM;

    // glScissor restringe limpeza e desenho a esse retângulo específico da tela;
    // sem isso, o glClear abaixo apagaria a cena principal já renderizada neste frame
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, TAMANHO_MINIMAPA, TAMANHO_MINIMAPA);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // fundo escuro para o minimapa se destacar da cena
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    // glViewport redireciona para onde o pipeline de rasterização desenha: a
    // partir daqui, as coordenadas normalizadas (-1 a 1) do shader são mapeadas
    // só para esse quadrado no canto da tela, não para a janela inteira
    glViewport(x, y, TAMANHO_MINIMAPA, TAMANHO_MINIMAPA);

    // Câmera olhando de cima para baixo: posição bem acima da vila (Y=30),
    // mirando a origem. O "up" é (0,0,-1) em vez de (0,1,0) porque, olhando
    // reto para baixo, o vetor para cima da tela precisa apontar para -Z do
    // mundo (senão a imagem do minimapa sai espelhada)
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

    // Projeção ortográfica: ao contrário da perspectiva, não há ponto de fuga,
    // então a distância da câmera não altera o tamanho aparente dos objetos --
    // é o que dá a aparência de "mapa visto de cima" em vez de uma foto aérea
    glm::mat4 proj = glm::ortho(-ALCANCE_MUNDO, ALCANCE_MUNDO, -ALCANCE_MUNDO, ALCANCE_MUNDO, 0.1f, 100.0f);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

    // Reaproveita a posição/escala/rotação "base" de cada objeto (sem as
    // animações de pá/nuvem, que são aplicadas só na cena principal em main.cpp)
    // Simplificação justificada, já que no radar visto de cima essa diferença é imperceptível
    for (const auto& obj : objetos) {
        glm::mat4 model = calculaModelBase(obj);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj.textureId);
        glBindVertexArray(obj.vao);
        glDrawArrays(GL_TRIANGLES, 0, obj.nVertices);
    }

    // Restaura a viewport para a tela cheia, para a cena principal do próximo frame não ficar restrita
    glViewport(0, 0, screenWidth, screenHeight);
}
