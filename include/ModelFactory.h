#ifndef __MODELFACTORY__
#define __MODELFACTORY__

#include "Model.h"

class ModelFactory {
public:
    static DrawableComponent loadModel(const std::filesystem::path& path);
};

class CubeModelFactory {
public:
    static DrawableComponent createCube(const Color& color);
};

#endif