#version 400

// UV interpolada recebida do vertex shader (um valor diferente por fragmento,
// calculado automaticamente pela GPU a partir dos 3 vértices do triângulo)
in vec2 uv;

// Cor final deste fragmento (pixel), enviada para o framebuffer
out vec4 fragColor;

// Sampler2D = "canal" de leitura da textura ativa no slot GL_TEXTURE0
// (configurado via glActiveTexture/glBindTexture antes de cada glDrawArrays)
uniform sampler2D tex;

// Ligado pela tecla R (ver interaction.cpp); true só para os objetos do tipo
// CASA, CELEIRO e MOINHO no frame atual (main.cpp decide isso por objeto antes
// de cada draw call, com glUniform1i)
uniform bool usarCorRosa;

void main() {
    if (usarCorRosa) {
        // Ignora a textura e pinta o fragmento inteiro com um rosa sólido
        // (R=1.0, G=0.4, B=0.7, A=1.0 -- totalmente opaco)
        fragColor = vec4(1.0, 0.4, 0.7, 1.0);
    } else {
        // Comportamento normal: busca a cor da textura na coordenada UV deste fragmento
        fragColor = texture(tex, uv);
    }
}
