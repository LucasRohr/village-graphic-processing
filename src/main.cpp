#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Includes dos módulos da vila serão habilitados conforme cada frente for integrada aqui
#include "common/ObjetoCena.h"
#include "common/shaders.h"
// #include "camera-interacao/camera.h"
// #include "camera-interacao/animation.h"
// #include "camera-interacao/interaction.h"
// #include "camera-interacao/minimap.h"
// #include "geometria-vila/primitives.h"
#include "geometria-vila/village.h"
#include "geometria-vila/texture.h"
// #include "geometria-vila/obj_loader.h"

GLFWwindow* Window = nullptr;
int WIDTH = 1200;
int HEIGHT = 800;

std::vector<ObjetoCena> Objetos;
GLuint ShaderProgram = 0;

// Chamado pela GLFW sempre que a janela é redimensionada; guarda o novo tamanho
// para que viewport e projeção (câmera) acompanhem a janela nas próximas etapas
void redimensionaCallback(GLFWwindow* window, int w, int h) {
    WIDTH = w;
    HEIGHT = h;
}

// Inicializa GLFW, cria a janela com contexto OpenGL 3.3 Core e carrega os ponteiros
// de função da placa de vídeo via GLAD (sem isso nenhuma chamada gl* funciona)
void inicializaOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar o GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // obrigatório no macOS

    Window = glfwCreateWindow(WIDTH, HEIGHT, "Vila Virtual - Processamento Grafico", NULL, NULL);
    if (!Window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowSizeCallback(Window, redimensionaCallback);
    glfwMakeContextCurrent(Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar o GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Placa de video: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Versao do OpenGL: " << glGetString(GL_VERSION) << std::endl;
}

// Carrega texturas, monta a vila e compila o shader. Provisório: ainda não
// depende de camera.h, então roda mesmo antes dessa frente estar pronta
void inicializaCena() {
    TexturasVila texturas = carregaTexturasVila();
    geraVila(Objetos, texturas);

    ShaderProgram = compilaShaderProgram(
        "../assets/shaders/vertex_shader.glsl",
        "../assets/shaders/fragment_shader.glsl"
    );
}

// Laço principal. Por enquanto usa uma câmera FIXA só para validar a geometria,
// texturas e posicionamento da vila -- será substituída pela câmera livre de
// camera.h assim que essa frente estiver pronta (ver TODO abaixo)
void loopRenderizacao() {
    glEnable(GL_DEPTH_TEST); // necessário assim que houver objetos 3D, para não desenhar fora de ordem

    // TODO (próxima etapa, pessoa 1): trocar essa view/proj fixas pela câmera
    // livre de camera.h, recalculada a cada frame com base no input do usuário
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 15.0f, 25.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    while (!glfwWindowShouldClose(Window)) {
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.4f, 0.6f, 0.9f, 1.0f); // azul-céu provisório
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 200.0f);

        glUseProgram(ShaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

        for (auto& obj : Objetos) {
            glm::mat4 model = calculaModelBase(obj);
            glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, obj.textureId);

            glBindVertexArray(obj.vao);
            glDrawArrays(GL_TRIANGLES, 0, obj.nVertices);
        }

        // TODO (próxima etapa, pessoa 1): trataTeclado(), atualizaDirecaoCamera(),
        // animação (moinho/nuvens) e minimapa (minimap.h)

        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(Window, true);
        }

        glfwPollEvents();
        glfwSwapBuffers(Window);
    }

    glfwTerminate();
}

int main() {
    inicializaOpenGL();
    inicializaCena();
    loopRenderizacao();
    return 0;
}