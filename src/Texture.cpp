#include "Texture.h"

#include <glad/glad.h> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdexcept>
#include <sstream>

Texture::Texture(std::string_view sourceFileName, int format) : key{0} {
    unsigned char* data = stbi_load(sourceFileName.data(), &width, &height, &numberOfChannels, 0);
    if(data == NULL) {
        std::ostringstream ss;
        ss << "Could not load image data from " << sourceFileName;
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
}

Texture& Texture::operator=(Texture&& other) {
    std::swap(key, other.key);
    return *this;
}

Texture::~Texture() {
    if(key != 0) {
        glDeleteTextures(1, &key);
    }
}

void Texture::bind(unsigned int textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, key);
}