#ifndef MAINLOOP_HPP
#define MAINLOOP_HPP

#pragma once

#include <chrono>
#include "glm/mat4x4.hpp"

#include "support/arcball_camera.hpp"

#include "Constants.hpp"
#include "graphmodule.hpp"
#include "support/ConfigParser.h"

#include "meshslicer/include/pointTransformer.h"
#include "meshslicer/include/wavefrontObjSerializer.h"
#include "meshslicer/include/planeDescriptionSerializer.h"
#include "meshslicer/include/meshCreator.h"
#include "meshslicer/include/meshSlicer.h"

#include "string"



class MainLoop
{
public:
    MainLoop();

    GraphModule graphModule;

    MeshData originalMesh;
    MeshSlicer slicer;
    MeshMirror mirror;



    glm::vec3 planeOrigin= glm::vec3{ 0.97f, 0.0f, 0.0f };
    glm::vec3 planeNormal= glm::vec3{ 1.0f, 0.0f, 0.3f };
    glm::vec3 planeNormalRotate= glm::vec3{ 1.0f, 0.0f, 0.3f };
    float angleOfRotationStep=0.5f;

    MeshData resultUpper, resultLower, resultMirror;

    bool RunLoop();

    ArcBallCamera camera_;

    struct {
      glm::mat4x4 view;
      glm::mat4x4 proj;
      glm::mat4x4 viewProj;
    } matrix_;

    std::chrono::steady_clock::time_point time_;
    float deltatime_;

    void update_camera();
    void update_time();

    TEventData camera_event;

    ConfigParser config;

    int tick=0;

    void configureMesh();
    void setup();

    bool Step(float deltaTime, int interTick);
    float rotationAngle=180;
     glm::mat4 trans;
     glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
     bool CreateNewMesh(float deltaTime, int interTick);
};

#endif // MAINLOOP_HPP
