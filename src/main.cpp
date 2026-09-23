#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Includes dos módulos da vila serão habilitados conforme cada frente for integrada aqui
// #include "common/ObjetoCena.h"
// #include "common/shaders.h"
// #include "camera-interacao/camera.h"
// #include "camera-interacao/animation.h"
// #include "camera-interacao/interaction.h"
// #include "camera-interacao/minimap.h"
// #include "geometria-vila/primitives.h"
// #include "geometria-vila/village.h"
// #include "geometria-vila/obj_loader.h"
// #include "geometria-vila/texture.h"

GLFWwindow* Window = nullptr;
int WIDTH = 1200;
int HEIGHT = 800;

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

// Laço principal. Por enquanto só limpa a tela a cada frame para validar que a
// janela e o contexto OpenGL estão funcionando; aqui entram, nas próximas etapas,
// o tratamento de teclado/mouse, a atualização da câmera e os draw calls da vila
void loopRenderizacao() {
    glEnable(GL_DEPTH_TEST); // necessário assim que houver objetos 3D, para não desenhar fora de ordem

    while (!glfwWindowShouldClose(Window)) {
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.4f, 0.6f, 0.9f, 1.0f); // azul-céu provisório, só para confirmar que a janela renderiza
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO (próxima etapa): trataTeclado(), atualizaDirecaoCamera(), desenho dos
        // objetos da vila (village.h), elemento animado e minimapa (minimap.h)

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
    loopRenderizacao();
    return 0;
}
