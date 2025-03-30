#ifndef __TEXTURE__
#define __TEXTURE__

#include <filesystem>

class Texture {
public:
    enum class Type {
        Diffuse,
        Specular
    };

    Texture(std::filesystem::path sourceFileName, Type type);
    Texture(const Texture& other) = delete;
    Texture(Texture&& other);
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other);
    ~Texture();

    void bind(unsigned int textureUnit) const;
    Type getType() const;
    const std::filesystem::path& getPath() const;
private:
    int width;
    int height;
    int numberOfChannels;
    unsigned int key;
    Type type;
    std::filesystem::path path;
};

#endif