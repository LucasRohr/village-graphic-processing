#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Registra o callback de movimento do mouse e configura a captura do cursor.
// Chamar uma única vez, depois que a janela (GLFWwindow) já foi criada
void inicializaCamera(GLFWwindow* window);

// Recalcula o vetor "para onde a câmera olha" a partir dos ângulos atuais de
// yaw/pitch. Chamar uma vez por frame, antes de montar a matriz de View
void atualizaDirecaoCamera();

// Lê o teclado (WASD + Q/E) e move a câmera. deltaTime é o tempo entre o frame
// atual e o anterior, usado para a velocidade não depender do framerate
void processaTecladoCamera(GLFWwindow* window, float deltaTime);

// Matriz de View: posição da câmera + direção para onde ela olha + vetor "up"
glm::mat4 getViewMatrix();

// Matriz de Projection em perspectiva. fovGraus = campo de visão vertical,
// aspecto = largura/altura da janela
glm::mat4 getProjectionMatrix(float fovGraus, float aspecto);

// Posição atual da câmera no mundo (usada pelo minimapa para desenhar o jogador)
glm::vec3 getCameraPos();

#endif
