#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

// Estado da câmera: fica restrito a este arquivo (static) e só é acessado de
// fora através das funções declaradas em camera.h
static glm::vec3 Cam_pos   = glm::vec3(0.0f, 3.0f, 18.0f); // começa afastada e um pouco acima do chão, olhando para a vila
static glm::vec3 Cam_front = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 Cam_up    = glm::vec3(0.0f, 1.0f, 0.0f);

static float Cam_yaw   = -90.0f; // -90 graus aponta a câmera para -Z (de frente para a vila)
static float Cam_pitch = 0.0f;
static float Cam_speed = 6.0f;   // unidades do mundo por segundo

// Guardam a última posição do cursor para calcular o deslocamento (delta) entre frames
static double lastX = 0.0, lastY = 0.0;
static bool primeiroMouse = true;

// Callback chamado automaticamente pela GLFW a cada movimento do mouse.
// window = janela que gerou o evento; xpos/ypos = posição atual do cursor em
// pixels, relativa ao canto superior esquerdo da área de desenho da janela
static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (primeiroMouse) {
        // Na primeira leitura ainda não existe "posição anterior" válida;
        // só guardamos a atual como referência, sem mover a câmera neste frame
        lastX = xpos;
        lastY = ypos;
        primeiroMouse = false;
    }

    float xoffset = (float)(xpos - lastX);
    float yoffset = (float)(lastY - ypos); // invertido: eixo Y da tela cresce para baixo, mundo cresce para cima
    lastX = xpos;
    lastY = ypos;

    float sensibilidade = 0.1f; // reduz o deslocamento bruto do mouse (em pixels) para um giro suave
    xoffset *= sensibilidade;
    yoffset *= sensibilidade;

    Cam_yaw   += xoffset;
    Cam_pitch += yoffset;

    // Trava o pitch entre -89 e 89 graus para a câmera nunca "virar de cabeça para baixo" (evitar Gimbal lock)
    if (Cam_pitch > 89.0f)  Cam_pitch = 89.0f;
    if (Cam_pitch < -89.0f) Cam_pitch = -89.0f;
}

void inicializaCamera(GLFWwindow* window) {
    // GLFW_CURSOR_DISABLED esconde o cursor do sistema operacional e permite que
    // ele se mova indefinidamente (sem travar na borda da janela), do jeito
    // necessário para uma câmera livre em primeira pessoa
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Registra mouseCallback como a função chamada pela GLFW sempre que o
    // cursor se move dentro da janela
    glfwSetCursorPosCallback(window, mouseCallback);
}

void atualizaDirecaoCamera() {
    // Converte os ângulos de yaw (giro no eixo Y) e pitch (inclinação) em um
    // vetor de direção normalizado, via coordenadas esféricas
    glm::vec3 front;
    front.x = cos(glm::radians(Cam_yaw)) * cos(glm::radians(Cam_pitch));
    front.y = sin(glm::radians(Cam_pitch));
    front.z = sin(glm::radians(Cam_yaw)) * cos(glm::radians(Cam_pitch));
    Cam_front = glm::normalize(front);
}

void processaTecladoCamera(GLFWwindow* window, float deltaTime) {
    float velocidade = Cam_speed * deltaTime; // distância percorrida neste frame específico

    // Vetor "direita" da câmera: perpendicular a Front e Up, via produto vetorial
    glm::vec3 Cam_right = glm::normalize(glm::cross(Cam_front, Cam_up));

    // glfwGetKey devolve GLFW_PRESS enquanto a tecla estiver segurada, então o
    // movimento é contínuo enquanto W/A/S/D/Q/E ficam pressionadas
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) Cam_pos += Cam_front * velocidade;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) Cam_pos -= Cam_front * velocidade;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) Cam_pos -= Cam_right * velocidade;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) Cam_pos += Cam_right * velocidade;

    // Sobe/desce no eixo Y global, independente de para onde a câmera está olhando
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) Cam_pos.y += velocidade;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) Cam_pos.y -= velocidade;
}

glm::mat4 getViewMatrix() {
    // lookAt monta a matriz de visualização a partir de 3 vetores: posição do
    // "olho" (a câmera), o ponto para onde ele olha, e o vetor "para cima"
    return glm::lookAt(Cam_pos, Cam_pos + Cam_front, Cam_up);
}

glm::mat4 getProjectionMatrix(float fovGraus, float aspecto) {
    // near=0.1 e far=200 definem os planos de corte: nada mais perto que 0.1 ou
    // mais longe que 200 unidades é renderizado
    return glm::perspective(glm::radians(fovGraus), aspecto, 0.1f, 200.0f);
}

glm::vec3 getCameraPos() {
    return Cam_pos;
}
