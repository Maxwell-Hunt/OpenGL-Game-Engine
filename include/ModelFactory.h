#ifndef __MODELFACTORY__
#define __MODELFACTORY__

#include "Model.h"

class ModelFactory {
public:
    static DrawableComponent loadModel(const std::filesystem::path& path, LightingType lightingType = LightingType::Phong);
};

class CubeModelFactory {
public:
    static DrawableComponent createCube(const Color& color, LightingType lightingType = LightingType::Phong);
};

#endif