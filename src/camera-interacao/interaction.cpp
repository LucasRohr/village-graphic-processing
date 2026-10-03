#include "interaction.h"

// Estado restrito a este arquivo: true enquanto o modo rosa estiver ligado
static bool corRosaLigada = false;

// Callback chamado pela GLFW a cada evento de tecla. Parâmetros:
// window   = janela que gerou o evento
// key      = código da tecla (uma constante GLFW_KEY_*)
// scancode = código bruto do sistema operacional para a tecla (não usado aqui)
// action   = GLFW_PRESS (pressionou agora), GLFW_RELEASE (soltou) ou GLFW_REPEAT
//            (ficou segurando e o SO reenviou o evento, tipo tecla de texto)
// mods     = teclas modificadoras pressionadas junto (shift/ctrl/alt)
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Só alterna no instante exato em que a tecla é pressionada (GLFW_PRESS).
    // Isso é o "debounce": se reagíssemos também a GLFW_REPEAT, segurar a tecla
    // faria corRosaLigada ligar/desligar repetidamente, piscando a cor
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        corRosaLigada = !corRosaLigada;
    }
}

void inicializaInteracao(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
}

bool corRosaAtiva() {
    return corRosaLigada;
}
