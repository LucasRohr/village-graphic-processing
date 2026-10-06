#version 400

// location 0 = posição local do vértice (X,Y,Z)
// location 1 = coordenada de textura (U,V), variando de 0.0 a 1.0
layout(location = 0) in vec3 vertexPosicao;
layout(location = 1) in vec2 vertexUV;

// model = transformação do objeto (posição/rotação/escala)
// view  = posição e orientação da câmera
// proj  = projeção (perspectiva na cena principal, ortográfica no minimapa)
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

// Repassa a UV para o fragment shader; o valor é interpolado automaticamente
out vec2 uv;

void main() {
    uv = vertexUV;
    // Combina as 3 matrizes (multiplicação da direita para a esquerda)
    gl_Position = proj * view * model * vec4(vertexPosicao, 1.0);
}
