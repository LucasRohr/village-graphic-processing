#version 400

// UV interpolada recebida do vertex shader
in vec2 uv;

// Cor final deste fragmento (pixel), enviada para o framebuffer
out vec4 fragColor;

// Sampler2D = "canal" de leitura da textura ativa
uniform sampler2D tex;

// Ligado pela tecla R (ver interaction.cpp); true só para os objetos do tipo CASA, CELEIRO e MOINHO
uniform bool usarCorRosa;

void main() {
    if (usarCorRosa) {
        // Ignora a textura e pinta o fragmento inteiro com um rosa sólido
        fragColor = vec4(1.0, 0.4, 0.7, 1.0);
    } else {
        // Comportamento normal: busca a cor da textura na coordenada UV deste fragmento
        fragColor = texture(tex, uv);
    }
}
