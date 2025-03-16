#ifndef __TEXTURE__
#define __TEXTURE__

#include <string>

class Texture {
public:
    enum class Type {
        Diffuse,
        Specular
    };

    Texture(std::string_view sourceFileName, Type type);
    Texture(const Texture& other) = delete;
    Texture(Texture&& other);
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other);
    ~Texture();

    void bind(unsigned int textureUnit) const;
    Type getType() const;
private:
    int width;
    int height;
    int numberOfChannels;
    unsigned int key;
    Type type;
};

#endif