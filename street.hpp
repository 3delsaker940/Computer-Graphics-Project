#pragma once
#include "textured-shape.hpp" 

namespace Example {
    class Street : public TexturedShape {
    public:
        Street();         void setupStreet();         void draw();     };
}