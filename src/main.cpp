#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/ObjetoCena.h"
#include "common/shaders.h"
#include "camera-interacao/camera.h"
#include "camera-interacao/animation.h"
#include "camera-interacao/interaction.h"
#include "camera-interacao/minimap.h"
#include "geometria-vila/primitives.h"
#include "geometria-vila/village.h"
#include "geometria-vila/texture.h"
#include "geometria-vila/obj_loader.h"

GLFWwindow* Window = nullptr;
int WIDTH = 1200;
int HEIGHT = 800;

std::vector<ObjetoCena> Objetos;
GLuint ShaderProgram = 0;

// Chamado pela GLFW sempre que a janela é redimensionada; guarda o novo tamanho
// para que viewport e projeção (câmera) acompanhem a janela
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

// Carrega texturas, monta a vila (geometria + animais em .obj) e compila o shader
void inicializaCena() {
    TexturasVila texturas = carregaTexturasVila();
    geraVila(Objetos, texturas);
    carregaAnimais(Objetos);

    ShaderProgram = compilaShaderProgram(
        "../assets/shaders/vertex_shader.glsl",
        "../assets/shaders/fragment_shader.glsl"
    );
}

// Monta a matriz Model de um objeto, aplicando as animações contínuas quando
// o tipo exige (pá do moinho girando, nuvem se deslocando). Para os demais
// tipos (incluindo os animais .obj), delega para calculaModelBase (village.cpp),
// sem duplicar a lógica padrão
glm::mat4 calculaModelAnimado(const ObjetoCena& obj, float tempo) {
    glm::mat4 model = glm::mat4(1.0f);

    if (obj.tipo == MOINHO_PA) {
        // Translada até a posição da pá e gira em torno do eixo Z local (o eixo
        // "para fora" do moinho), antes da rotação Y de orientação do objeto e da escala
        model = glm::translate(model, obj.posicao);
        model = glm::rotate(model, glm::radians(calculaAnguloPa(tempo)), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(obj.rotacaoY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(obj.escala));
        return model;
    }

    if (obj.tipo == NUVEM) {
        // Usa a posição animada (com wrap-around em X) no lugar da posição fixa do objeto
        glm::vec3 posAnimada = calculaPosicaoNuvem(obj.posicao, tempo);
        model = glm::translate(model, posAnimada);
        model = glm::rotate(model, glm::radians(obj.rotacaoY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(obj.escala));
        return model;
    }

    return calculaModelBase(obj);
}

// Laço principal: trata input, atualiza a câmera, desenha a vila (com animações
// e o efeito da tecla R) e, por cima, o minimapa no canto da tela
void loopRenderizacao() {
    glEnable(GL_DEPTH_TEST); // necessário para objetos 3D não se sobreporem fora de ordem

    double tempoAnterior = glfwGetTime();

    while (!glfwWindowShouldClose(Window)) {
        // deltaTime = tempo entre este frame e o anterior; usado para a
        // velocidade da câmera não depender da taxa de quadros da máquina
        double tempoAtual = glfwGetTime();
        float deltaTime = (float)(tempoAtual - tempoAnterior);
        tempoAnterior = tempoAtual;
        float tempo = (float)tempoAtual;

        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.4f, 0.6f, 0.9f, 1.0f); // azul-céu
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Input e câmera: lê teclado (o mouse já é tratado via callback,
        // registrado em inicializaCamera) e recalcula a direção para onde a câmera olha
        processaTecladoCamera(Window, deltaTime);
        atualizaDirecaoCamera();

        glm::mat4 view = getViewMatrix();
        glm::mat4 proj = getProjectionMatrix(60.0f, (float)WIDTH / (float)HEIGHT);

        glUseProgram(ShaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

        // Localização do uniform (bool) que liga/desliga a cor rosa no fragment shader
        GLint locCorRosa = glGetUniformLocation(ShaderProgram, "usarCorRosa");

        for (auto& obj : Objetos) {
            glm::mat4 model = calculaModelAnimado(obj, tempo);
            glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            // A tecla R só afeta casa, celeiro e moinho (não terreno, pá, árvore, animais etc.)
            bool aplicaRosa = corRosaAtiva() && (obj.tipo == CASA || obj.tipo == CELEIRO || obj.tipo == MOINHO);
            glUniform1i(locCorRosa, aplicaRosa ? 1 : 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, obj.textureId);

            glBindVertexArray(obj.vao);
            glDrawArrays(GL_TRIANGLES, 0, obj.nVertices);
        }

        // Segunda passada de renderização, restrita a um canto da tela (radar de navegação)
        desenhaMinimapa(Objetos, ShaderProgram, WIDTH, HEIGHT);

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
    inicializaCamera(Window);     // configura mouse livre (cursor escondido + callback)
    inicializaInteracao(Window);  // registra o callback de teclado da tecla R
    inicializaCena();
    loopRenderizacao();
    return 0;
}