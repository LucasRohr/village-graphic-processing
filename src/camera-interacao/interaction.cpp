#include "interaction.h"

// Estado restrito a este arquivo: true enquanto o modo rosa estiver ligado
static bool corRosaLigada = false;

// Callback chamado pela a cada evento de tecla. Parâmetros:
// window   = janela que gerou o evento
// key      = código da tecla
// action   = GLFW_PRESS (pressionado)
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Só alterna no instante exato em que a tecla é pressionada (GLFW_PRESS)
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        corRosaLigada = !corRosaLigada;
    }
}

// Registra o callback de teclado responsável pelas interações da cena (tecla R pressionada)
void inicializaInteracao(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
}

bool corRosaAtiva() {
    return corRosaLigada;
}
