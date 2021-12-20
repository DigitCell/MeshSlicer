#include "graphmodule.hpp"


GraphModule::GraphModule()
{
    if(!InitSDL())
        printf("Some problems in inint SDL");

    if(!InitImGui())
        printf("Some problems in inint ImGui");

    shaderutils.InitGL();

    InitShaders();
    Setup();

}

bool GraphModule::InitShaders()
{

    /* Setup programs */
    char *src_buffer = new char[MAX_SHADER_BUFFERSIZE]();
    pgm_.basic = shaderutils.CreateRenderProgram((path_string+"shaders/"+ "basic/vs_basic.glsl").data(),
                                     (path_string+"shaders/"+ "basic/fs_basic.glsl").data(),
                                     src_buffer);

    pgm_.basic_color = shaderutils.CreateRenderProgram((path_string+"shaders/"+ "basic/vs_basic_color.glsl").data(),
                                     (path_string+"shaders/"+ "basic/fs_basic_color.glsl").data(),
                                     src_buffer);

    pgm_.grid = shaderutils.CreateRenderProgram((path_string+"shaders/"+ "grid/vs_grid.glsl").data(),
                                    (path_string+"shaders/"+ "grid/fs_grid.glsl").data(),
                                    src_buffer);

    pgm_.terrain = shaderutils.CreateRenderProgram((path_string+"shaders/"+ "terrain/vs_terrain.glsl").data(),
                                    (path_string+"shaders/"+ "terrain/fs_terrain.glsl").data(),
                                    src_buffer);
    delete [] src_buffer;

    /* Shaders uniform location */
    ulocation_.basic.color        = shaderutils.GetUniformLocation(pgm_.basic, "uColor");
    ulocation_.basic.mvp          = shaderutils.GetUniformLocation(pgm_.basic, "uMVP");
    ulocation_.basic.scaleFactor   = shaderutils.GetUniformLocation(pgm_.basic, "uScaleFactor");

    ulocation_.basic_color.mvp           = shaderutils.GetUniformLocation(pgm_.basic_color, "uMVP");
    ulocation_.basic_color.scaleFactor   = shaderutils.GetUniformLocation(pgm_.basic_color, "uScaleFactor");

    ulocation_.grid.mvp           = shaderutils.GetUniformLocation(pgm_.grid, "uMVP");
    ulocation_.grid.scaleFactor   = shaderutils.GetUniformLocation(pgm_.grid, "uScaleFactor");

    ulocation_.terrain.mvp           = shaderutils.GetUniformLocation(pgm_.terrain, "uMVP");
    ulocation_.terrain.modelView     = shaderutils.GetUniformLocation(pgm_.terrain, "uModelView");
    ulocation_.terrain.color        =  shaderutils.GetUniformLocation(pgm_.terrain, "uColor");
    ulocation_.terrain.scaleFactor   = shaderutils.GetUniformLocation(pgm_.terrain, "uScaleFactor");

    return true;
}

bool GraphModule::Setup()
{
    setup_grid_geometry();
    setup_wirecube_geometry();
    return true;
}


bool GraphModule::InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return false;
    }

    const char* glsl_version = "#version 150";
    // Initialize rendering context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);


    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    screen = SDL_CreateWindow("SPH", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mainScreenWidth, mainScreenHeight, window_flags);
    gContext = SDL_GL_CreateContext(screen);

    SDL_GL_MakeCurrent(screen, gContext);
    SDL_GL_SetSwapInterval(1);

    GLenum err;
    glewExperimental = GL_TRUE; // Please expose OpenGL 3.x+ interfaces
    err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to init GLEW" << std::endl;
        SDL_GL_DeleteContext(gContext);
        SDL_DestroyWindow(screen);
        SDL_Quit();
        return true;
    }


    //Main loop flag
    bool quit = false;
    SDL_Event e;
    SDL_StartTextInput();

    int major, minor;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
    std::cout << "OpenGL version       | " << major << "." << minor << std::endl;
    std::cout << "GLEW version         | " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "---------------------+-------" << std::endl;

    return true;
}

bool GraphModule::InitImGui()
{
    // Setup Dear ImGui context
       IMGUI_CHECKVERSION();
       ImGui::CreateContext();
       ImGui::LoadIniSettingsFromDisk("tempImgui.ini");
       ImGuiIO& io = ImGui::GetIO(); (void)io;
       static ImGuiStyle* style = &ImGui::GetStyle();
       style->Alpha = 1.00f; //0.75f

       io.WantCaptureMouse=true;
       //io.WantCaptureKeyboard=false;
       //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
       //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

       // Setup Dear ImGui style
       ImGui::StyleColorsDark();
       //ImGui::StyleColorsClassic();

       // Setup Platform/Renderer backends
       ImGui_ImplSDL2_InitForOpenGL(screen, gContext);
       ImGui_ImplOpenGL3_Init();

       return true;
}


void GraphModule::ClearRenderScreen(){
    if(clearScreen)
    {
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void GraphModule::Render(glm::mat4x4 const &view, glm::mat4x4 const& viewProj){

    glm::mat4x4 mvp;
    glm::mat4x4 model;
    glm::vec4 color;

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    // -- SOLID OBJECTS

    //Grid
    if (debug_parameters_.show_grid)
      draw_grid(viewProj);

      //model=glm::translate(viewProj, glm::vec3(10.0,0,0));
     // model=glm::scale(model, glm::vec3(10.0,1.0,1.0));
     // draw_wirecube(viewProj, glm::vec4(1.0,1.0,1.0,1.0));

    // -- TRANSPARENT OBJECTS

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

}

void GraphModule::RenderGrid( glm::mat4x4 const& viewProj){

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    //Grid
    if (debug_parameters_.show_grid)
      draw_grid(viewProj);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

}


void GraphModule::GuiRender( MeshData& mesh, glm::vec3& planeOrigin, glm::vec3& planeNormal,  glm::vec3& planeNormalRotate, float& angleOfRotation)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(screen);
    ImGui::NewFrame();

    {
        ImGui::Begin("Mesh parameters");

            ImGui::Text("Framerate  : %.1f ms or %.1f Hz", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Separator();

            ImGui::Checkbox("Run simulation",&runSimulation);
            ImGui::Checkbox("Save Data", &saveData);

            ImGui::SliderFloat("Plane origin x", &planeOrigin.x, -17.0, 17.0);
            ImGui::SliderFloat("Plane origin y", &planeOrigin.y, -17.0, 17.0);
            ImGui::SliderFloat("Plane origin z", &planeOrigin.z, -17.0, 17.0);
            ImGui::Separator();


            ImGui::SliderFloat("Plane rotate x", &planeNormal.x, -1.0, 1.0);
            ImGui::SliderFloat("Plane rotate y", &planeNormal.y, -1.0, 1.0);
            ImGui::SliderFloat("Plane rotate z", &planeNormal.z, -1.0, 1.0);
            ImGui::Separator();


            ImGui::SliderFloat("axis rotate x", &planeNormalRotate.x, -1.0, 1.0);
            ImGui::SliderFloat("axis rotate y", &planeNormalRotate.y, -1.0, 1.0);
            ImGui::SliderFloat("axis rotate z", &planeNormalRotate.z, -1.0, 1.0);
            ImGui::Separator();


            ImGui::SliderFloat("mesh offset x", &meshOffset.x, -500.0, 500.0);
            ImGui::SliderFloat("mesh offset y", &meshOffset.y, -500.0, 500.0);
            ImGui::SliderFloat("mesh offset z", &meshOffset.z, -500.0, 500.0);
            ImGui::Separator();
            ImGui::SliderFloat("step angle of rotation", &angleOfRotation, -0.5, 0.5);
            ImGui::Separator();
            ImGui::Separator();


            if(ImGui::Button("Next Step"))
                    nextStep=true;
            if(ImGui::Button("Stop simulation"))
                runSimulation=false;

            ImGui::Separator();
            ImGui::Separator();

            ImGui::Checkbox("draw mesh centroid",&drawCentroidMesh);
            ImGui::Checkbox("draw mesh half",&drawHalf);
            ImGui::Checkbox("draw mesh mirror",&drawMirror);
            ImGui::Checkbox("draw mesh main",&drawMeshMain);

            needUpdateLineBuffer=ImGui::Checkbox("wireframe draw", &drawWireFrame);
            ImGui::Separator();
            ImGui::Separator();

            if(ImGui::Button("Create new mesh"))
                CreateNewMesh=true;


            ImGui::Separator();

            if(ImGui::Button("Plane 1 0 0"))
                planeNormal=glm::vec3(1,0,0);

            if(ImGui::Button("Plane 0 1 0"))
                planeNormal=glm::vec3(0,1,0);

            if(ImGui::Button("Plane 0 0 1"))
                planeNormal=glm::vec3(0,0,1);

            if(ImGui::Button("Origin 0 0 1"))
                planeOrigin=glm::vec3(0,0,0);


            if(ImGui::Button("axis rotate 1 0 0"))
                planeNormalRotate=glm::vec3(1,0,0);

            if(ImGui::Button("axis rotate 0 1 0"))
                planeNormalRotate=glm::vec3(0,1,0);

            if(ImGui::Button("axis rotate 0 0 1"))
                planeNormalRotate=glm::vec3(0,0,1);





        ImGui::End();

        ImVec2 uv0 = ImVec2(10.0f/256.0f, 10.0f/256.0f);

        // Normalized coordinates of pixel (110,210) in a 256x256 texture.
        ImVec2 uv1 = ImVec2((10.0f+100.0f)/256.0f, (10.0f+200.0f)/256.0f);

        ImGui::Begin("Texture 2 viewer");
           // ImGui::Text("pointer = %p", mSharedTex->mId);
           // ImGui::Text("size = %d x %d", mSharedTex->mWidth,  mSharedTex->mHeight);
           // ImGui::Image((void*)(intptr_t)mSharedTex->mId, ImVec2(mSharedTex->mWidth, mSharedTex->mHeight));
        ImGui::End();

    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



void GraphModule::CloseRender()
{
    //close program, return true

    glDeleteVertexArrays(1u, &geo_.grid.vao);
    glDeleteVertexArrays(1u, &geo_.wirecube.vao);
    glDeleteVertexArrays(1u, &geo_.meshbase.vao);
    glDeleteBuffers(1u, &geo_.grid.vbo);
    glDeleteBuffers(1u, &geo_.wirecube.vbo);
    glDeleteBuffers(1u, &geo_.wirecube.ibo);
    glDeleteBuffers(1u, &geo_.meshbase.vbo);
    glDeleteBuffers(1u, &geo_.meshbase.vbo_n);
    glDeleteBuffers(1u, &geo_.meshbase.ibo);

    SDL_StopTextInput();
    SDL_DestroyWindow(screen);
    screen = NULL;
    SDL_Quit();

}

void  GraphModule::HandleEvents(SDL_Event e, TEventData& camera_event){

   bool &bRotatePressed = camera_event.bRotatePressed;
   if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT)
       bRotatePressed=true;
   if(e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT)
       bRotatePressed=false;
   //bRotatePressed = bRotatePressed || (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT);
  // bRotatePressed = bRotatePressed && (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT);

   bool &bTranslatePressed = camera_event.bTranslatePressed;
   if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_MIDDLE)
       bTranslatePressed=true;
   if(e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_MIDDLE)
       bTranslatePressed=false;
   //bTranslatePressed = bTranslatePressed || (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_MIDDLE);
   //bTranslatePressed = bTranslatePressed && (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_MIDDLE);

   if(e.type==SDL_MOUSEMOTION){
       camera_event.mouseX = static_cast<float>(e.motion.x);
       camera_event.mouseY = static_cast<float>(e.motion.y);
       camera_event.bMouseMove = true;
   }

   if(e.type==SDL_MOUSEWHEEL)
   {
       camera_event.wheelDelta=0.5f*e.wheel.y;
   }

   if (e.type == SDL_KEYDOWN){
       if (e.key.keysym.sym == SDLK_LCTRL){
           bTranslatePressed=true;
       }
   }

   if (e.type == SDL_KEYUP){
       if (e.key.keysym.sym == SDLK_LCTRL){
           bTranslatePressed=false;
       }
   }

}



void GraphModule::setup_grid_geometry() {
  // size taken in world space
  float const world_size = 1.0f;

  unsigned int const &res          = 32u; //static_cast<unsigned int>(gpu_particle_->simulation_box_size()) / 2u;
  unsigned int const &num_vertices = 4u * (res + 1u);
  unsigned int const num_component = 2u;
  unsigned int const buffersize    = num_vertices * num_component;
  std::vector<float> vertices(buffersize);

  geo_.grid.resolution = res;
  geo_.grid.nvertices  = static_cast<GLsizei>(num_vertices); //

  float const cell_padding = world_size / res;
  float const offset = cell_padding * (res/2.0f);

  for (unsigned int i=0u; i<=res; ++i) {
    unsigned int const index = 4u * num_component * i;
    float const cursor = cell_padding * i - offset;

    // horizontal lines
    vertices[index + 0u] = - offset;
    vertices[index + 1u] = cursor;
    vertices[index + 2u] = + offset;
    vertices[index + 3u] = cursor;
    // vertical lines
    vertices[index + 4u] = cursor;
    vertices[index + 5u] = - offset;
    vertices[index + 6u] = cursor;
    vertices[index + 7u] = + offset;
  }

  // Allocate Storage.
  glGenBuffers(1u, &geo_.grid.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, geo_.grid.vbo);
    GLsizeiptr const bytesize = static_cast<GLsizeiptr>(vertices.size() * sizeof(vertices[0u]));
    glBufferStorage(GL_ARRAY_BUFFER, bytesize, vertices.data(), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0u);

  // Set attribute locations.
  glGenVertexArrays(1u, &geo_.grid.vao);
  glBindVertexArray(geo_.grid.vao);
  {
    size_t const attrib_size = num_component * sizeof(vertices[0u]);
    glBindVertexBuffer(0, geo_.grid.vbo, 0u, attrib_size);
    {
      size_t const attrib_pos = 0u;
      glVertexAttribFormat(attrib_pos, num_component, GL_FLOAT, GL_FALSE, 0);
      glVertexAttribBinding(attrib_pos, 0);
      glEnableVertexAttribArray(attrib_pos);
    }
  }
  glBindVertexArray(0u);
}

void GraphModule::setup_wirecube_geometry() {
  // Setup the wireframe cube

  float const world_size = 1.0f;
  float const c = 0.5f * world_size;

  std::array<float, 24> const vertices = {
    {+c, +c, +c,   +c, -c, +c,   +c, -c, -c,   +c, +c, -c,
    -c, +c, +c,   -c, -c, +c,   -c, -c, -c,   -c, +c, -c}
  };

  std::array<unsigned char, 24> const indices = {
    {0, 1, 1, 2, 2, 3, 3, 0,
    4, 5, 5, 6, 6, 7, 7, 4,
    0, 4, 1, 5, 2, 6, 3, 7}
  };

  // Vertices storage.
  GLsizeiptr bytesize(0);
  glGenBuffers(1u, &geo_.wirecube.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, geo_.wirecube.vbo);
    bytesize = vertices.size() * sizeof(vertices[0u]);
    glBufferStorage(GL_ARRAY_BUFFER, bytesize, vertices.data(), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0u);

  // Indices storage.
  glGenBuffers(1u, &geo_.wirecube.ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geo_.wirecube.ibo);
    bytesize = indices.size() * sizeof(indices[0u]);
    glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, bytesize, indices.data(), 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);

  // Rendering attributes.
  glGenVertexArrays(1u, &geo_.wirecube.vao);
  glBindVertexArray(geo_.wirecube.vao);
  {
    // Positions
    unsigned int const attrib_pos = 0u;
    glBindVertexBuffer(attrib_pos, geo_.wirecube.vbo, 0, 3u*sizeof(vertices[0u]));
    glVertexAttribFormat(attrib_pos, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(attrib_pos, 0);
    glEnableVertexAttribArray(attrib_pos);

    // Enable element array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geo_.wirecube.ibo);
  }
  glBindVertexArray(0u);

  // Element array parameters.
  geo_.wirecube.nindices = static_cast<unsigned int>(indices.size());
  geo_.wirecube.indices_type = GL_UNSIGNED_BYTE;

  CHECKGLERROR();
}




void GraphModule::setup_texture() {
#if 0
  unsigned int const res = sprite_width*sprite_height;
  char *pixels = new char[3u*res];
  char *texdata = new char[res];

  for (unsigned int i=0u; i < res; ++i) {
    char *px = pixels + 3*i;
    HEADER_PIXEL(sprite_data, px);
    texdata[i] = *px;
  }
  delete [] pixels;

  GLsizei const w = static_cast<GLsizei>(sprite_width);
  GLsizei const h = static_cast<GLsizei>(sprite_height);
  glGenTextures(1u, &gl_sprite_tex_);
  glBindTexture(GL_TEXTURE_2D, gl_sprite_tex_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexStorage2D(GL_TEXTURE_2D, 4u, GL_R8, w, h);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RED, GL_UNSIGNED_BYTE, texdata);
    glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0u);

  delete [] texdata;

  // bind here just for testing.
  glBindTexture(GL_TEXTURE_2D, gl_sprite_tex_);

  CHECKGLERROR();
#endif
}

void GraphModule::draw_grid(glm::mat4x4 const &mvp) {
  float constexpr kDefaultSimulationVolumeSize = 128.0f;
  glUseProgram(pgm_.grid);
  {
    glUniformMatrix4fv(ulocation_.grid.mvp, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform1f(ulocation_.grid.scaleFactor, kDefaultSimulationVolumeSize);

    glBindVertexArray(geo_.grid.vao);
      glDrawArrays(GL_LINES, 0, geo_.grid.nvertices);
    glBindVertexArray(0u);
  }
  glUseProgram(0u);

  CHECKGLERROR();
}


void GraphModule::draw_wirecube(glm::mat4x4 const &mvp, glm::vec4 const &color) {
  glUseProgram(pgm_.basic);
  {
    glUniformMatrix4fv(ulocation_.basic.mvp, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform4fv(ulocation_.basic.color, 1u, glm::value_ptr(color));

    glBindVertexArray(geo_.wirecube.vao);
      glDrawElements(GL_LINES, geo_.wirecube.nindices, geo_.wirecube.indices_type, nullptr);
    glBindVertexArray(0u);
  }
  glUseProgram(0u);

  CHECKGLERROR();
}


void GraphModule::RenderMeshCut(GEO::MeshBase_& meshIn,glm::vec3 translate, glm::mat4x4 const &view, glm::mat4x4 const& viewProj){
    glm::mat4x4 mvp;
    glm::vec4 color=glm::vec4(0.3,1.0,0.5,1.0);
    glm::vec4 color2=glm::vec4(0.3,1.0,1.0,1.0);

    const float radius_scale=0.95f;

    glm::mat4x4 model;
    model=glm::translate(viewProj, translate);
    model=glm::scale(model, glm::vec3(radius_scale,radius_scale,radius_scale));

    draw_mesh(meshIn, translate, model, glm::vec4(0.3,1.0,0.5,1.0), true);

    CHECKGLERROR();

}


void GraphModule::draw_mesh(GEO::MeshBase_& meshIn, glm::vec3 translate, glm::mat4x4 const &mvp, glm::vec4 const &color, bool bFill) {
  GLenum mode = (bFill) ? GL_TRIANGLE_STRIP : GL_LINES;

  glm::mat4 model(1.0);

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);

  const glm::vec3 plane=glm::vec3(1,0,0);

  model=glm::translate(initView, glm::vec3(0.0, 0.0,-200.0));
  model=glm::rotate(model, glm::radians(30.0f), plane);
  model=glm::scale(model, glm::vec3(0.3,0.3,0.3));
/*
  glUseProgram(pgm_.terrain);
  {
      glUniformMatrix4fv(ulocation_.terrain.mvp, 1, GL_FALSE, glm::value_ptr(mvp));
      glUniformMatrix4fv(ulocation_.terrain.modelView, 1, GL_FALSE, glm::value_ptr(model));
      glUniform4fv(ulocation_.terrain.color, 1u, glm::value_ptr(color));
      glUniform1f(ulocation_.terrain.scaleFactor, kDefaultSimulationVolumeSize);

     //glLineWidth(12.0f);
     glBindVertexArray(meshIn.vao);
         //  glDrawElements(GL_POINTS, meshIn.nindices-1, GL_UNSIGNED_INT, 0 );
               glDrawElements(GL_LINES, meshIn.nindices, GL_UNSIGNED_INT, 0 );
         //    glDrawElements(GL_TRIANGLES, meshIn.nindices , GL_UNSIGNED_INT, 0);
     glBindVertexArray(0u);
  }
  glUseProgram(0u);
  */

  glUseProgram(pgm_.basic_color);
  {
      glUniformMatrix4fv(ulocation_.basic_color.mvp, 1, GL_FALSE, glm::value_ptr(mvp));
      glUniform1f(ulocation_.basic_color.scaleFactor, kDefaultSimulationVolumeSize);

     //glLineWidth(12.0f);
     glBindVertexArray(meshIn.vao);

          // glDrawElements(GL_POINTS, meshIn.nindices, GL_UNSIGNED_INT, 0 );
         //     glDrawElements(GL_LINES, meshIn.nindices, GL_UNSIGNED_INT, 0 );

         if(drawWireFrame)
            glDrawElements(GL_LINES, meshIn.nindices, GL_UNSIGNED_INT, 0 );
         else
            glDrawElements(GL_TRIANGLES, meshIn.nindices , GL_UNSIGNED_INT, 0);
     glBindVertexArray(0u);
  }
  glUseProgram(0u);

  CHECKGLERROR();
}



void GraphModule::update_mesh_geometry(GEO::MeshBase_& meshIn,  MeshData& mesh) {

    meshIn.nvertices=mesh.numberVertices();
    meshIn.nindices=mesh.numberIndices();

    // Vertices buffer.
    {
        glBindBuffer(GL_ARRAY_BUFFER, meshIn.vbo);
          size_t  bytesize = meshIn.nvertices * 3*sizeof(float);
          glBufferData(GL_ARRAY_BUFFER, bytesize, mesh.vertices.data(),  GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0u);
    }
    // Color buffer
    {
        glBindBuffer(GL_ARRAY_BUFFER, meshIn.vbo_n);
          size_t  bytesize = meshIn.nvertices * 4*sizeof(float);
          glBufferData(GL_ARRAY_BUFFER, bytesize, mesh.colors.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0u);
    }

    // Indices buffer.
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIn.ibo);
          size_t bytesize = meshIn.nindices* sizeof(uint);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesize, mesh.triangelVertices.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
    }

}



void GraphModule::update_indices_for_Line_geometry(GEO::MeshBase_& meshIn,  MeshData& mesh) {

    meshIn.nindices=mesh.lineIndices.size();
    // Indices buffer.
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIn.ibo);
          size_t bytesize = meshIn.nindices* sizeof(uint);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesize, mesh.lineIndices.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
    }
}


void GraphModule::update_indices_for_Triangle_geometry(GEO::MeshBase_& meshIn,  MeshData& mesh) {
    meshIn.nindices=mesh.numberIndices();
    // Indices buffer.
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIn.ibo);
          size_t bytesize = meshIn.nindices* sizeof(uint);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesize, mesh.triangelVertices.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
    }
}

void GraphModule::setup_mesh_geometry(GEO::MeshBase_& meshIn,  MeshData& mesh) {

   unsigned int const num_component = 3u;

   meshIn.nvertices=mesh.numberVertices();
   meshIn.nindices=mesh.numberIndices();


  // Vertices buffer.
  {
      glGenBuffers(1u, &meshIn.vbo);
      glBindBuffer(GL_ARRAY_BUFFER, meshIn.vbo);
        size_t  bytesize = meshIn.nvertices * 3*sizeof(float);
        glBufferData(GL_ARRAY_BUFFER, bytesize, mesh.vertices.data(),  GL_DYNAMIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, 0u);
  }
  // Color buffer
  {
      glGenBuffers(1u, &meshIn.vbo_n);
      glBindBuffer(GL_ARRAY_BUFFER, meshIn.vbo_n);
        size_t  bytesize = meshIn.nvertices * 4*sizeof(float);
        glBufferData(GL_ARRAY_BUFFER, bytesize, mesh.colors.data(), GL_DYNAMIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, 0u);
  }

  // Indices buffer.
  {
      glGenBuffers(1u, &meshIn.ibo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIn.ibo);
        size_t bytesize = meshIn.nindices* sizeof(uint);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesize, mesh.triangelVertices.data(), GL_DYNAMIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
  }

  // Set attribute locations.
  glGenVertexArrays(1u, &meshIn.vao);
  glBindVertexArray(meshIn.vao);
  {

      {
           size_t const attrib_size = 3 * sizeof(float);
           unsigned int const attrib_pos = 0u;
           glBindVertexBuffer(attrib_pos, meshIn.vbo, 0, attrib_size);
           glVertexAttribFormat(attrib_pos, 3, GL_FLOAT, GL_FALSE, 0);
           glVertexAttribBinding(attrib_pos,0);
           glEnableVertexAttribArray(attrib_pos);
      }

      {
           size_t const attrib_size = 4 * sizeof(float);
           unsigned int const attrib_pos = 1u;
           glBindVertexBuffer(attrib_pos,meshIn.vbo_n, 0, attrib_size);
           glVertexAttribFormat(attrib_pos, 4, GL_FLOAT, GL_FALSE, 0);
           glVertexAttribBinding(attrib_pos,1);
           glEnableVertexAttribArray(attrib_pos);
      }

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIn.ibo);

  }
  glBindVertexArray(0u);


   CHECKGLERROR();

   //delete [] pmv;
}
