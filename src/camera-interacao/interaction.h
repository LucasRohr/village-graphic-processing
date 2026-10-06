#ifndef INTERACTION_H
#define INTERACTION_H

#include <GLFW/glfw3.h>

// Registra o callback de teclado responsável pelas interações da cena (tecla R pressionada)
void inicializaInteracao(GLFWwindow* window);

// true enquanto o modo "rosa" estiver ligado (casas, celeiros e moinhos trocam de cor)
bool corRosaAtiva();

#endif
