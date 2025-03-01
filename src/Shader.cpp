#include <fstream>
#include <sstream>
#include <iostream>
#include "Shader.h"

Shader::Shader(std::string_view sourceFileName, GLenum type) :
    key{glCreateShader(type)}
{
    std::ifstream vertexShaderFile(sourceFileName.data());
    if(!vertexShaderFile.is_open()) {
        std::ostringstream ss;
        ss << "Could not load shader from " << sourceFileName;
        throw std::runtime_error(ss.str());
    }
    std::stringstream bf; bf << vertexShaderFile.rdbuf();
    std::string sourceStr = bf.str(); // Intermediate string necessary sicne bf.str() returns a temporary string
    const char* source = sourceStr.c_str();
    glShaderSource(key, 1, &source, NULL);
    glCompileShader(key);

    checkCompilation(sourceFileName);
}

Shader::Shader(Shader&& other) {
    std::swap(key, other.key);
}

Shader& Shader::operator=(Shader&& other) {
    std::swap(key, other.key);
    return *this;
}

Shader::~Shader() {
    if(key != 0) {
        glDeleteShader(key);
    }
}

void Shader::checkCompilation(std::string_view fileName) const { 
    int success;
    char infoLog[512];
    glGetShaderiv(key, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(key, 512, NULL, infoLog);
        std::ostringstream ss;
        ss << "SHADER COMPILATION FAILED (" << fileName << ")\n";
        throw std::runtime_error(ss.str() + infoLog);
    }
}

unsigned int Shader::getKey() const {
    return key;
}

ShaderProgram::ShaderProgram() : key{glCreateProgram()} {}

ShaderProgram::ShaderProgram(ShaderProgram&& other) {
    std::swap(key, other.key);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) {
    std::swap(key, other.key);
    return *this;
}

ShaderProgram::~ShaderProgram() {
    if(key != 0) {
        glDeleteProgram(key);
    }
}

ShaderProgram& ShaderProgram::attach(Shader&& shader) {
    glAttachShader(key, shader.getKey());
    return *this;
}

void ShaderProgram::link() const {
    glLinkProgram(key);
    checkProgramLinking();
}

void ShaderProgram::checkProgramLinking() const {
    int success;
    char infoLog[512];
    glGetProgramiv(key, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(key, 512, NULL, infoLog);
        throw std::runtime_error("PROGRAM LINKING FAILED " + std::string(infoLog));
    }
}

void ShaderProgram::use() const {
    glUseProgram(key);
}

void ShaderProgram::setBool(std::string_view name, bool value) const {
    glUniform1i(glGetUniformLocation(key, name.data()), (int)value);
}

void ShaderProgram::setInt(std::string_view name, int value) const {
    glUniform1i(glGetUniformLocation(key, name.data()), value);
}

void ShaderProgram::setFloat(std::string_view name, float value) const {
    glUniform1f(glGetUniformLocation(key, name.data()), value);
}

void ShaderProgram::setFloat(std::string_view name, float value1, float value2) const {
    glUniform2f(glGetUniformLocation(key, name.data()), value1, value2);
}

void ShaderProgram::setFloat(std::string_view name, float value1, float value2, float value3) const {
    glUniform3f(glGetUniformLocation(key, name.data()), value1, value2, value3);
}

void ShaderProgram::setFloat(std::string_view name, float value1, float value2, float value3, float value4) const {
    glUniform4f(glGetUniformLocation(key, name.data()), value1, value2, value3, value4);
}

void ShaderProgram::setMat4(std::string_view name, float* value, bool transpose) {
    glUniformMatrix4fv(glGetUniformLocation(key, name.data()), 1, transpose, value);
}