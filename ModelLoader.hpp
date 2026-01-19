#pragma once
#include <string>
#include <glm/glm.hpp>
#include "basic-shape.hpp"

namespace Example {
    void loadModelToShape(BasicShape& outShape, const std::string& path, glm::vec3 color);
}