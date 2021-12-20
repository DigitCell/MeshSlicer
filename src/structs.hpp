#include "Constants.hpp"

#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include <glm/vec3.hpp>

#pragma once

struct TEventData {
  float wheelDelta;
  float mouseX;
  float mouseY;
  bool bMouseMove;
  bool bRotatePressed;
  bool bTranslatePressed;
  bool bSpacePressed;
};

struct Color
{
    float r = 1.f, g = 1.f, b = 1.f;
};


struct Box {
    float x, y, z;
};
