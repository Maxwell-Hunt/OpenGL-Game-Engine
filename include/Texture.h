#ifndef __TEXTURE__
#define __TEXTURE__

#include <string>

class Texture {
public:
    Texture(std::string_view sourceFileName);
    Texture(const Texture& other) = delete;
    Texture(Texture&& other);
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other);
    ~Texture();

    void bind(unsigned int textureUnit);
private:
    int width;
    int height;
    int numberOfChannels;
    unsigned int key;
};

#endif