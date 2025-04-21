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

void ShaderProgram::setMat4(std::string_view name, float* value, bool transpose) const {
    glUniformMatrix4fv(glGetUniformLocation(key, name.data()), 1, transpose, value);
}

void ShaderProgram::setLight(const std::string& name, Light value) const {
    std::string s(name);
    setFloat(s+".color", value.color[0], value.color[1], value.color[2]);
    setFloat(s+".ambientStrength", value.ambientStrength);
    setFloat(s+".diffuseStrength", value.diffuseStrength);
    setFloat(s+".specularStrength", value.specularStrength);
}

void ShaderProgram::setDirectionalLight(std::string_view name, DirectionalLight value) const {
    std::string s(name);
    setLight(s, value);
    setFloat(s+".direction", value.direction[0], value.direction[1], value.direction[2]);
}

void ShaderProgram::setPointLight(std::string_view name, PointLightComponents value) const {
    std::string s(name);
    setLight(s, value.pointLight);
    setFloat(s+".position", value.transform.position[0], value.transform.position[1], value.transform.position[2]);
    setFloat(s+".kc", value.pointLight.kc);
    setFloat(s+".kl", value.pointLight.kl);
    setFloat(s+".kq", value.pointLight.kq);
}
