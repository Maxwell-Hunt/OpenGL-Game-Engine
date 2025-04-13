#ifndef __MODELFACTORY__
#define __MODELFACTORY__

#include "Model.h"

class ModelFactory {
public:
    static Model loadModel(const std::filesystem::path& path);
};

class CubeModelFactory {
public:
    static CubeModel createCube(const Color& color);
};

#endif