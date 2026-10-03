#ifndef INTERACTION_H
#define INTERACTION_H

#include <GLFW/glfw3.h>

// Registra o callback de teclado responsável pelas interações pontuais da cena
// (diferente de processaTecladoCamera, que roda a cada frame para o movimento
// contínuo da câmera -- aqui reagimos só ao instante em que a tecla é apertada)
void inicializaInteracao(GLFWwindow* window);

// true enquanto o modo "rosa" estiver ligado (casas, celeiros e moinhos trocam de cor)
bool corRosaAtiva();

#endif
