#include "shaders.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string leShaderDoArquivo(const char* caminho) {
    std::ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        std::cerr << "ERRO: Nao foi possivel abrir o arquivo do shader: " << caminho << std::endl;
        return "";
    }

    std::stringstream stream;
    stream << arquivo.rdbuf(); // despeja o conteúdo inteiro do arquivo no stream
    arquivo.close();
    return stream.str();
}

// Compila um shader individual (vertex ou fragment) e imprime o log de erro se falhar.
// tipo = GL_VERTEX_SHADER ou GL_FRAGMENT_SHADER; codigoFonte = texto do .glsl já lido do disco
static GLuint compilaShader(GLenum tipo, const char* codigoFonte) {
    GLuint shader = glCreateShader(tipo); // reserva um identificador de shader do tipo informado
    // glShaderSource(shader, count, &string, length): count=1 porque passamos
    // o código inteiro numa única string; length=NULL assume que a string é
    // terminada em '\0' (não precisa informar o tamanho manualmente)
    glShaderSource(shader, 1, &codigoFonte, NULL);
    glCompileShader(shader);

    GLint success;
    char infoLog[512]; // 512 bytes é espaço de sobra para a maioria das mensagens de erro do driver
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // pergunta ao driver se a compilação deu certo (success = GL_TRUE/GL_FALSE)
    if (!success) {
        // glGetShaderInfoLog(shader, tamanhoMaximoDoBuffer, tamanhoRealEscrito, buffer)
        // -- aqui passamos NULL no terceiro parâmetro porque não precisamos saber o tamanho exato, só o texto
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Erro ao compilar shader:\n" << infoLog << std::endl;
    }
    return shader;
}

GLuint compilaShaderProgram(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = leShaderDoArquivo(vertexPath);
    std::string fragmentCode = leShaderDoArquivo(fragmentPath);

    GLuint vs = compilaShader(GL_VERTEX_SHADER, vertexCode.c_str());
    GLuint fs = compilaShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

    // Linka os dois shaders compilados em um único programa executável na GPU:
    // cria o program, anexa os dois shaders a ele e resolve as ligações entre
    // as saídas do vertex shader e as entradas do fragment shader (ex: "uv")
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success); // mesma ideia do GL_COMPILE_STATUS, mas para a etapa de linkagem
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Erro na linkagem do shader program:\n" << infoLog << std::endl;
    }

    // Os shaders individuais já foram linkados no program (uma cópia interna
    // fica associada a ele), não precisam mais existir soltos -- libera a memória
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
