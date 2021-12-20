#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#pragma once

#include <GL/glew.h>
#include "structs.hpp"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "glm/ext.hpp"
#include "glm/glm.hpp"

#include <SDL2/SDL.h>
#include "string"

/* OpenGL debug macro */
#ifdef NDEBUG
# define CHECKGLERROR()
#else
# define CHECKGLERROR()    CheckGLError(__FILE__, __LINE__, "", true)
#endif


#define NOISE_SSB_BP         0

#define PI  3.14159215
#define PHI 1.61803398


static float constexpr kDefaultSimulationVolumeSize = 30.0f;
static const std::string  path_string="../SDL_mslicer01.5/";

static
const char* GetErrorString(GLenum err) {
#define STRINGIFY(x) #x
  switch (err)
  {
    // [GetError]
    case GL_NO_ERROR:
      return STRINGIFY(GL_NO_ERROR);

    case GL_INVALID_ENUM:
      return STRINGIFY(GL_INVALID_ENUM);

    case GL_INVALID_VALUE:
      return STRINGIFY(GL_INVALID_VALUE);

    case GL_INVALID_OPERATION:
      return STRINGIFY(GL_INVALID_OPERATION);

    case GL_STACK_OVERFLOW:
      return STRINGIFY(GL_STACK_OVERFLOW);

    case GL_STACK_UNDERFLOW:
      return STRINGIFY(GL_STACK_UNDERFLOW);

    case GL_OUT_OF_MEMORY:
      return STRINGIFY(GL_OUT_OF_MEMORY);

    default:
      return "GetErrorString : Unknown constant";
  }
#undef STRINGIFY
}

inline void CheckGLError(const char* file, const int line, const char* errMsg, bool bExitOnFail) {
  GLenum err = glGetError();

  if (err != GL_NO_ERROR) {
    fprintf(stderr,
            "OpenGL error @ \"%s\" [%d] : %s [%s].\n",
            file, line, errMsg, GetErrorString(err));

    if (bExitOnFail) {
      exit(EXIT_FAILURE);
    }
  }
}

#endif // CONSTANTS_HPP
