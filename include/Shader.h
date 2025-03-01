#ifndef __SHADER__
#define __SHADER__

#include <glad/glad.h>
#include <string>
#include <array>
#include <optional>

/*
These classes serve to encapsulate the process of setting up Shaders and Shader Programs

TODO (Problems):
    There are a few problems with this currently. First of all, to attach a shader to a program
    you have to move it, and then the Shader program will own the shader. The reason things are
    set up like this is because it guarantees that the shader will not be destroyed before the
    program is linked which would cause us to use a ressource which doesn't exist. The reason this
    is not ideal is that it prevents a shader from being written to multiple shader programs.

    Secondly, a Shader won't currently be destroyed until the program that owns it is destroyed but this
    is a problem because best practice is to immediately delete the shader after linking. This is done so
    that the shader source code is not just sitting on the GPU taking up space when it doesn't need to be

Potential Fixes:
    Perhaps we could make it so that Shaders contain a static referencce counting system so that ownership
    could be shared between different ShaderPrograms. We could also make it so that the source code retrieval
    and the filename storage and the type storage happens in the Shader constructor, but glCreateShader is not
    actually called we are attached to at least one shader program. It would make sense that the ShaderProgram
    should release ownership when linking is done.
*/

class Shader {
public:
    // The sourceFileName parameter is expected to be NULL TERMINATED
    Shader(std::string_view sourceFileName, GLenum type);
    Shader(const Shader& other) = delete;
    Shader(Shader&& other);
    Shader& operator=(const Shader& other) = delete;
    Shader& operator=(Shader&& other);
    ~Shader();

    unsigned int getKey() const;
private:
    void checkCompilation(std::string_view fileName) const;

    unsigned int key;
};

class ShaderProgram {
public:
    static constexpr std::size_t MAX_SHADERS = 5;
    ShaderProgram();
    ShaderProgram(const ShaderProgram& other) = delete;
    ShaderProgram(ShaderProgram&& other);
    ShaderProgram& operator=(const ShaderProgram& other) = delete;
    ShaderProgram& operator=(ShaderProgram&& other);
    ~ShaderProgram();

    ShaderProgram& attach(Shader&& shader);
    void link() const;
    void use() const;

    void setBool(std::string_view name, bool value) const;
    void setInt(std::string_view name, int value) const;
    void setFloat(std::string_view name, float value) const;
    void setFloat(std::string_view name, float value1, float value2) const;
    void setFloat(std::string_view name, float value1, float value2, float value3) const;
    void setFloat(std::string_view name, float value1, float value2, float value3, float value4) const;
    void setMat4(std::string_view name, float* value, bool transpose = false);
private:
    void checkProgramLinking() const;

    unsigned int key;
};

#endif