#version 400
in vec3 cores;

out vec4 frag_colour;

void main () {
    frag_colour = vec4(cores, 1.0);
}