#ifndef GRAPHMODULE_HPP
#define GRAPHMODULE_HPP

#pragma once

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "Constants.hpp"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <SDL2/SDL.h>
#include "iostream"
#include "support/shaderutils.hpp"

#include "vector"
#include "array"
#include "stdio.h"

#include "support/Icosphere.h"

#include "meshslicer/include/pointTransformer.h"
#include "meshslicer/include/wavefrontObjSerializer.h"
#include "meshslicer/include/planeDescriptionSerializer.h"
#include "meshslicer/include/meshCreator.h"
#include "meshslicer/include/meshSlicer.h"


class GraphModule
{
public:
    GraphModule();

    ShaderUtils shaderutils;

    struct DebugParameters_t {
      bool show_grid = true;
      bool coordinates = true;
      bool show_simulation_volume = true;
      bool freeze = false;
    };

    DebugParameters_t debug_parameters_;

    bool InitSDL();
    bool InitImGui();
    bool InitShaders();

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glm::mat4x4 initView;

    int mainScreenWidth=2100;
    int mainScreenHeight=1500;

    float f = 0.0f;
    int counter = 0;

    SDL_Window* screen = NULL;
    SDL_GLContext gContext;
    SDL_Event event;

    bool runSimulation=false;
    bool restartSimulation=false;
    bool clearScreen=true;

    bool saveData=false;
    bool nextStep=false;
    bool drawMesh=true;
    bool drawHalf=true;
    bool drawMeshMain=true;

    bool drawCentroidMesh=false;
    bool drawWireFrame=false;
    bool drawMirror=true;
    bool needUpdateLineBuffer=false;
    bool needUpdateTriangleBuffer=false;

    bool CreateNewMesh=false;

    glm::vec3 meshOffset=glm::vec3(190.0,0,0);

    void ClearRenderScreen();


    struct GEO {
        struct {
            GLuint vao;
            GLuint vbo;
            GLsizei nvertices;
            unsigned int resolution;
        } grid;

        struct {
            GLuint vao;
            GLuint vbo;
            GLuint ibo;
            GLenum indices_type;
            GLsizei nindices;
        } wirecube;

        struct {
            GLuint vao;
            GLuint vbo;
            GLuint ibo;
            GLsizei nvertices;
            GLenum indices_type;
            GLsizei nindices;
            unsigned int resolution;
        } terrain;


        struct MeshBase_{
            GLuint vao;
            GLuint vbo;
            GLuint vbo_n;
            GLuint ibo;
            GLsizei nvertices;
            GLenum indices_type;
            GLsizei nindices;
            unsigned int resolution;
        } meshbase;

        MeshBase_ meshleft;
        MeshBase_ meshright;
        MeshBase_ meshMirror;

    } geo_;

    struct {
        GLuint basic;
        GLuint basic_color;
        GLuint grid;
        GLuint terrain;
    } pgm_;

    struct {
        struct {
            GLint mvp;
            GLint color;
            GLint scaleFactor;
        } basic;
        struct {
            GLint mvp;
            GLint scaleFactor;
        } basic_color;
        struct {
            GLint mvp;
            GLint scaleFactor;
        } grid;
        struct {
            GLint mvp;
            GLint modelView;
            GLint color;
            GLint scaleFactor;
        } terrain;

    } ulocation_;


    void CloseRender();

    void setup_grid_geometry();
    void setup_wirecube_geometry();
    void draw_wirecube(const glm::mat4x4 &mvp, const glm::vec4 &color);
    void draw_grid(const glm::mat4x4 &mvp);
    void HandleEvents(SDL_Event e, TEventData &camera_event);
    void Render(const glm::mat4x4 &view, const glm::mat4x4 &viewProj);

    void setup_texture();

    bool Setup();
    void setup_mesh_geometry(GEO::MeshBase_& meshIn, MeshData  &mesh);

    void RenderMeshCut(GEO::MeshBase_ &meshIn, glm::vec3 translate, const glm::mat4x4 &view, const glm::mat4x4 &viewProj);
    void update_mesh_geometry(GEO::MeshBase_ &meshIn, MeshData  &mesh);
    void draw_mesh(GEO::MeshBase_ &meshIn, glm::vec3 translate, const glm::mat4x4 &mvp, const glm::vec4 &color, bool bFill);
    void update_indices_for_Line_geometry(GEO::MeshBase_ &meshIn, MeshData &mesh);
    void update_indices_for_Triangle_geometry(GEO::MeshBase_ &meshIn, MeshData &mesh);
    void RenderGrid(const glm::mat4x4 &viewProj);


    void GuiRender(MeshData &mesh, glm::vec3 &planeOrigin, glm::vec3 &planeNormal, glm::vec3 &planeNormalRotate, float &angleOfRotation);
};

#endif // GRAPHMODULE_HPP
