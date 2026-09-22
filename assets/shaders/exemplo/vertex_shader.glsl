#version 400
layout(location = 0) in vec3 vertex_posicao;
layout(location = 1) in vec3 vertex_cores;

out vec3 cores;

void main () {
    cores = vertex_cores;
    gl_Position = vec4(vertex_posicao, 1.0);
}