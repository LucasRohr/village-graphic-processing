#version 400
in vec2 uv;
out vec4 fragColor;

uniform sampler2D tex;
uniform bool usarCorRosa; // ligado pela tecla R (interaction.cpp), só para casa/celeiro/moinho

void main() {
    if (usarCorRosa) {
        // Ignora a textura e pinta o objeto inteiro de rosa sólido
        fragColor = vec4(1.0, 0.4, 0.7, 1.0);
    } else {
        fragColor = texture(tex, uv);
    }
}
