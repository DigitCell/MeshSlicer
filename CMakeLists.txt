cmake_minimum_required(VERSION 3.14)

set(GLM_DIR  ${CMAKE_SOURCE_DIR}/external/glm-0.9.9.8/cmake/glm)

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_LIBRARY_DIRS})
add_definitions(${GLM_DEFINITIONS})

# SDL2, glew, glm, ImGui
add_subdirectory("external")
set(EXECUTABLE_NAME "SwingLine")

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)

project(${EXECUTABLE_NAME}
    VERSION     0.15
    DESCRIPTION "SDL2 OpenGL Marching Cubes Boids "
    LANGUAGES CXX
)

set( SPDLOG_LIB_PATH_INC ${PROJECT_SOURCE_DIR}/external/spdlog/include)
set( RAPIDJSON_LIB_PATH_INC ${PROJECT_SOURCE_DIR}/external/rapidjson/include)

# IMGUI LIB
set (IMGUILIB ImGuiLibrary)
set(IMGUI_ROOT ${CMAKE_SOURCE_DIR}/external/imgui-1.85)
set(IMGUI_SOURCES ${IMGUI_ROOT}/imgui.cpp ${IMGUI_ROOT}/imgui_draw.cpp ${IMGUI_ROOT}/imgui_widgets.cpp ${IMGUI_ROOT}/imgui_tables.cpp)
set(IMGUI_IMPL_SOURCES ${IMGUI_ROOT}/backends/imgui_impl_sdl.cpp ${IMGUI_ROOT}/backends/imgui_impl_opengl3.cpp)
add_library(${IMGUILIB} STATIC ${IMGUI_SOURCES} ${IMGUI_IMPL_SOURCES} )
target_include_directories(${IMGUILIB} PUBLIC "${IMGUI_ROOT}")
target_include_directories(${IMGUILIB} PUBLIC "${IMGUI_ROOT}/backends")

#SLICER LIB
set(MESHSLICER_PATH ${CMAKE_SOURCE_DIR}/src/meshslicer)

set(MESHSLICER_H

                ${MESHSLICER_PATH}/include/pointTransformer.h
                ${MESHSLICER_PATH}/include/commandLineParser.h
                ${MESHSLICER_PATH}/include/log.h
                ${MESHSLICER_PATH}/include/meshData.h
                ${MESHSLICER_PATH}/include/meshSlicer.h
                ${MESHSLICER_PATH}/include/meshCreator.h
                ${MESHSLICER_PATH}/include/utils.h

                )
set(MESHSLICER_SRC

                ${MESHSLICER_PATH}/src/pointTransformer.cpp
                ${MESHSLICER_PATH}/src/commandLineParser.cpp
                ${MESHSLICER_PATH}/src/log.cpp
                ${MESHSLICER_PATH}/src/meshData.cpp
                ${MESHSLICER_PATH}/src/meshSlicer.cpp

                )

set (THISLIB slicerLib)
add_library(${THISLIB} STATIC ${MESHSLICER_H} ${MESHSLICER_SRC} )

target_include_directories(${THISLIB} PRIVATE ${MESHSLICER_H})
target_include_directories(${THISLIB} PRIVATE ${RAPIDJSON_LIB_PATH_INC})
target_include_directories(${THISLIB} PRIVATE ${SPDLOG_LIB_PATH_INC})
target_include_directories(${THISLIB} PRIVATE ${GLM_LIB_PATH_INC})
target_link_libraries(${THISLIB} pthread)
target_sources(${THISLIB} PRIVATE ${SOURCE_FILES})

SET(TARGET_H

    src/Constants.hpp
    src/structs.hpp

    src/graphmodule.hpp
    src/mainloop.hpp

    src/support/shaderutils.hpp
    src/support/arcball_camera.hpp
    src/support/Buffer.h
    src/support/ComputeProcess.h
    src/support/ConfigParser.h
    src/support/Icosphere.h
    src/support/NoiseSettings.h

)

SET(TARGET_SRC

    src/graphmodule.cpp
    src/mainloop.cpp

    src/support/shaderutils.cpp
    src/support/Buffer.cpp
    src/support/ComputeProcess.cpp
    src/support/ConfigParser.cpp
    src/support/Icosphere.cpp
    src/support/NoiseSettings.cpp

    src/main.cpp
)

add_executable(${EXECUTABLE_NAME} ${TARGET_SRC} ${TARGET_H}  )
target_link_libraries(${EXECUTABLE_NAME} ${IMGUILIB} )
target_link_libraries(${EXECUTABLE_NAME} ${THISLIB}  )

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${SPDLOG_LIB_PATH_INC})
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${RAPIDJSON_LIB_PATH_INC})

set(homeDir "/home/geotyper")
set(stb_lib  ${homeDir}/CPP_lib/stb)
include_directories(${stb_lib})

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${GLM_LIBRARY_DIRS})

if(NOT GLM_FOUND)
        message(Error "GLM not found")
endif(NOT GLM_FOUND)

# Windows prereq: get dev libraries from https://www.libsdl.org/download-2.0.php
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
# workaround for CMP0004 issue with the SDL2 libraries on Linux
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
target_link_libraries(${EXECUTABLE_NAME} ${SDL2_LIBRARIES})

# OpenGL
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
if(NOT TARGET OpenGL::OpenGL)
  target_link_libraries(${EXECUTABLE_NAME} OpenGL::GL)
  target_include_directories(${EXECUTABLE_NAME} PUBLIC OpenGL::GL)
else()
  target_link_libraries(${EXECUTABLE_NAME} OpenGL::OpenGL)
  target_include_directories(${EXECUTABLE_NAME} PUBLIC OpenGL::OpenGL)
endif()

# GL Extension Wrangler (GLEW) get dev libraries from http://glew.sourceforge.net/
find_package(GLEW REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} "${GLEW_LIBRARIES}" -ldl -lpthread )
target_include_directories(${EXECUTABLE_NAME} PUBLIC "${GLEW_INCLUDE_DIRS}")

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/logs")
