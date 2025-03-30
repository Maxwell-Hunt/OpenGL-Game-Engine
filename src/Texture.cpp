#include "Texture.h"

#include <glad/glad.h> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdexcept>
#include <sstream>

Texture::Texture(std::filesystem::path sourceFileName, Texture::Type type) : key{0}, type{type}, path{sourceFileName} {
    unsigned char* data = stbi_load(sourceFileName.c_str(), &width, &height, &numberOfChannels, 0);
    if(data == NULL) {
        std::ostringstream ss;
        ss << "Could not load image data from " << sourceFileName;
        throw std::runtime_error(ss.str());
    }
    GLenum format;
    if(numberOfChannels == 1) format = GL_RED;
    else if(numberOfChannels == 3) format = GL_RGB;
    else if(numberOfChannels == 4) format = GL_RGBA;
    else {
        std::ostringstream ss;
        ss << "Could not determine format for " << sourceFileName;
        throw std::runtime_error(ss.str());
    }
    glGenTextures(1, &key);
    glBindTexture(GL_TEXTURE_2D, key);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

Texture::Texture(Texture&& other) {
    std::swap(key, other.key);
    std::swap(type, other.type);
    path = std::move(other.path);
}

Texture& Texture::operator=(Texture&& other) {
    std::swap(key, other.key);
    std::swap(type, other.type);
    path = std::move(other.path);
    return *this;
}

Texture::~Texture() {
    if(key != 0) {
        glDeleteTextures(1, &key);
    }
}

void Texture::bind(unsigned int textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, key);
}

Texture::Type Texture::getType() const {
    return type;
}

const std::filesystem::path& Texture::getPath() const {
    return path;
}