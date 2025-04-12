#include "Light.h"

#include <iostream>

int convertLightingTypeToInt(LightingType type) {
    switch(type) {
        case LightingType::Flat: return 0;
        case LightingType::Diffuse: return 1;
        case LightingType::Complete: return 2;
        default: throw std::runtime_error("Lighting Type Not Supported");
    }
}

LightingType convertIntToLightingType(int mode) {
    switch(mode) {
        case 0: return LightingType::Flat;
        case 1: return LightingType::Diffuse;
        case 2: return LightingType::Complete;
        default: 
            std::cerr << "Getting type: " << static_cast<int>(mode);
            throw std::runtime_error("That shading type is not supported");
    }
}