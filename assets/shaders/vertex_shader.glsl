#version 400

// Atributos de entrada, um por vértice, lidos do VBO ativo (ver glVertexAttribPointer
// em primitives.cpp/obj_loader.cpp): location 0 = posição local do vértice (X,Y,Z);
// location 1 = coordenada de textura (U,V), variando de 0.0 a 1.0
layout(location = 0) in vec3 vertexPosicao;
layout(location = 1) in vec2 vertexUV;

// Matrizes enviadas pela CPU a cada draw call (glUniformMatrix4fv em main.cpp/minimap.cpp):
// model = transformação do objeto (posição/rotação/escala no mundo)
// view  = posição e orientação da câmera
// proj  = projeção (perspectiva na cena principal, ortográfica no minimapa)
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

// Repassa a UV para o fragment shader; o valor é interpolado automaticamente
// entre os 3 vértices de cada triângulo para cada fragmento (pixel) gerado
out vec2 uv;

void main() {
    uv = vertexUV;
    // Combina as 3 matrizes (multiplicação da direita para a esquerda: primeiro
    // leva o vértice do espaço do objeto para o mundo via model, depois para o
    // espaço da câmera via view, depois projeta na tela via proj) e converte a
    // posição para vec4 (coordenada homogênea, w=1.0 indica "ponto", não "vetor")
    gl_Position = proj * view * model * vec4(vertexPosicao, 1.0);
}
