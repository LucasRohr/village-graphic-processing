#version 400
layout(location = 0) in vec3 vertexPosicao;
layout(location = 1) in vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 uv;

void main() {
    uv = vertexUV;
    gl_Position = proj * view * model * vec4(vertexPosicao, 1.0);
}