#include "Street.hpp"

namespace Example {
    Street::Street() {
        setupStreet();
    }

    void Street::setupStreet() {
  
        TexturedVertex vertices[] = {
       
            {{-5.0f, 0.0f, -50.0f},  {0.2f, 0.2f, 0.2f},    {0.0f, 0.0f}}, 
            {{ 5.0f, 0.0f, -50.0f},  {0.2f, 0.2f, 0.2f},    {1.0f, 0.0f}}, 
            {{ 5.0f, 0.0f,  50.0f},  {0.2f, 0.2f, 0.2f},    {1.0f, 1.0f}}, 
            {{-5.0f, 0.0f,  50.0f},  {0.2f, 0.2f, 0.2f},    {0.0f, 1.0f}}  
        };

        unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    }
}