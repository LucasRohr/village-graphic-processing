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

// Compila um shader individual (vertex ou fragment) e imprime o log de erro se falhar
static GLuint compilaShader(GLenum tipo, const char* codigoFonte) {
    GLuint shader = glCreateShader(tipo);
    glShaderSource(shader, 1, &codigoFonte, NULL);
    glCompileShader(shader);

    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
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

    // Linka os dois shaders compilados em um único programa executável na GPU
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Erro na linkagem do shader program:\n" << infoLog << std::endl;
    }

    // Os shaders individuais já foram linkados no program, não precisam mais existir soltos
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
