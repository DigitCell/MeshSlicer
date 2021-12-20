#include "mainloop.hpp"

MainLoop::MainLoop():deltatime_(0.0f), config(path_string+"config.txt")
{
    std::setbuf(stderr, nullptr);
    std::srand(static_cast<uint32_t>(std::time(nullptr)));

    camera_event.wheelDelta = 0.0f;
    camera_event.mouseX = 0.0f;
    camera_event.mouseY = 0.0f;
    camera_event.bMouseMove = false;
    camera_event.bRotatePressed = false;
    camera_event.bTranslatePressed = false;

    /* Camera setup. */
    camera_.dolly(2795.0f);

    int const w  = static_cast<int>(graphModule.mainScreenWidth);
    int const h = static_cast<int>(graphModule.mainScreenHeight);


    /* Setup the projection matrix */
    float const aspectRatio = w / static_cast<float>(h);
    matrix_.proj = glm::perspective(
      glm::radians(60.0f), aspectRatio, 0.01f, 5000.0f
    );


    graphModule.initView=matrix_.proj;
    /* Start the chrono. */
    time_ = std::chrono::steady_clock::now();

    setup();
}

glm::quat MainLoop::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest){
    start = normalize(start);
    dest = normalize(dest);

    float cosTheta = dot(start, dest);
    glm::vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f){
        // special case when vectors in opposite directions:
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
        if (glm::length2(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
            rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

        rotationAxis = normalize(rotationAxis);
        return glm::angleAxis(glm::radians(180.0f), rotationAxis);
    }

    rotationAxis = cross(start, dest);

    float s = sqrt( (1+cosTheta)*2 );
    float invs = 1 / s;

    return glm::quat(
        s * 0.5f,
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    );
}

bool MainLoop::Step(float deltaTime, int interTick)
{
    glm::vec4 Position(planeNormal.x, planeNormal.y, planeNormal.z, 1.0f);
    trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(angleOfRotationStep),
                        glm::normalize(glm::vec3(planeNormalRotate.x, planeNormalRotate.y, planeNormalRotate.z)));

    //rotationAngle-=1.0f;
    Position =Position* trans;

    planeNormal=glm::vec3(Position.x,Position.y,Position.z);

    //Slicer
    slicer.sliceFace(planeOrigin,planeNormal,resultUpper,resultLower);

    if(graphModule.saveData){
        graphModule.saveData=false;
         WavefrontObjSerializer::serialize("mirror_exp.obj",originalMesh);
       // WavefrontObjSerializer::serialize("cuttedMesh-upper.obj", resultUpper);
       // WavefrontObjSerializer::serialize("cuttedMesh-lower.obj", resultLower);
    }

    graphModule.update_mesh_geometry(graphModule.geo_.meshleft,  resultUpper);
    graphModule.update_mesh_geometry(graphModule.geo_.meshright, resultLower);

    //Mirror
    mirror.attachMeshPtr(std::make_shared<MeshData>(resultUpper));
    mirror.mirror(planeOrigin,planeNormal,resultMirror);
    graphModule.update_mesh_geometry(graphModule.geo_.meshMirror, resultMirror);

    if(graphModule.drawWireFrame)
    {
        originalMesh.recalculateLineIndices();
        graphModule.update_indices_for_Line_geometry(graphModule.geo_.meshbase, originalMesh);

        resultUpper.recalculateLineIndices();
        graphModule.update_indices_for_Line_geometry(graphModule.geo_.meshleft, resultUpper);

        resultLower.recalculateLineIndices();
        graphModule.update_indices_for_Line_geometry(graphModule.geo_.meshright, resultLower);

        resultMirror.recalculateLineIndices();
        graphModule.update_indices_for_Line_geometry(graphModule.geo_.meshMirror, resultMirror);
    }
}

bool MainLoop::CreateNewMesh(float deltaTime, int interTick)
{
    originalMesh=resultMirror;
    slicer.attachMeshPtr(std::make_shared<MeshData>(originalMesh));
    graphModule.update_mesh_geometry(graphModule.geo_.meshbase, originalMesh);


}

bool MainLoop::RunLoop()
{
    std::cout << "Running main loop" << std::endl;
    bool running = true;
    static Uint32 lastFrameEventTime = 0;
    const Uint32 debounceTime = 100; // 100ms

    int interTick=0;

    bool done = false;
    while (!done)
    {
        camera_event.bMouseMove = false;
        camera_event.wheelDelta = 0.0f;
        Uint32 curTime = SDL_GetTicks();
        while (SDL_PollEvent(&graphModule.event))
        {
            ImGui_ImplSDL2_ProcessEvent(&graphModule.event);
            ImGuiIO& io = ImGui::GetIO();
            if (graphModule.event.type == SDL_QUIT)
            {
                done = true;
                ImGui::SaveIniSettingsToDisk("tempImgui.ini");
            }
            if (graphModule.event.type == SDL_WINDOWEVENT && graphModule.event.window.event == SDL_WINDOWEVENT_CLOSE && graphModule.event.window.windowID == SDL_GetWindowID(graphModule.screen))
                done = true;
            else
            {
                graphModule.HandleEvents(graphModule.event, camera_event);
            }
        }

        /* Update chrono and calculate deltatim */
        update_time();

        /* Camera event handling and matrices update */
        update_camera();
        /* Clean Screen */
        graphModule.ClearRenderScreen();

    //    graphModule.Render(matrix_.view, matrix_.viewProj);
/*
        {
            graphModule.RenderGrid(matrix_.viewProj
                                   *glm::toMat4(RotationBetweenVectors(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)))
                                   *glm::rotate(trans, glm::radians(rotationAngle), glm::vec3(1.0f, 0.0f, 0.0f)));
        }
*/


        if(graphModule.runSimulation){
            Step(deltatime_, interTick);
            interTick++;
        }

        if(graphModule.nextStep){
             Step(deltatime_, interTick);
             graphModule.nextStep=false;
             interTick++;
        }

        if(graphModule.needUpdateLineBuffer and graphModule.drawWireFrame){
            graphModule.needUpdateLineBuffer=false;
            originalMesh.recalculateLineIndices();
            graphModule.update_indices_for_Line_geometry(graphModule.geo_.meshbase, originalMesh);

            resultUpper.recalculateLineIndices();
            graphModule.update_indices_for_Line_geometry(graphModule.geo_.meshleft, resultUpper);

            resultLower.recalculateLineIndices();
            graphModule.update_indices_for_Line_geometry(graphModule.geo_.meshright, resultLower);

            resultMirror.recalculateLineIndices();
            graphModule.update_indices_for_Line_geometry(graphModule.geo_.meshMirror, resultMirror);
        }
        else if(graphModule.needUpdateLineBuffer and !graphModule.drawWireFrame){
            graphModule.needUpdateLineBuffer=false;
            graphModule.update_indices_for_Triangle_geometry(graphModule.geo_.meshbase,originalMesh);
            graphModule.update_indices_for_Triangle_geometry(graphModule.geo_.meshleft,  resultUpper);
            graphModule.update_indices_for_Triangle_geometry(graphModule.geo_.meshright, resultLower);
            graphModule.update_indices_for_Triangle_geometry(graphModule.geo_.meshMirror, resultMirror);
        }

        if(graphModule.drawMeshMain)
        {
            glm::vec3 translateMesh=graphModule.meshOffset;
            graphModule.RenderMeshCut(graphModule.geo_.meshbase, translateMesh, matrix_.view, matrix_.viewProj);
        }

        if(graphModule.drawCentroidMesh){
            if(graphModule.drawHalf)
            {
                glm::vec3 translateMeshLeft=glm::vec3(0.0f,33.0f,00.0f);
                graphModule.RenderMeshCut(graphModule.geo_.meshleft,translateMeshLeft, matrix_.view, matrix_.viewProj);
                glm::vec3 translateMeshRight=glm::vec3(0.0f,33.0f,00.0f);
                graphModule.RenderMeshCut(graphModule.geo_.meshright,translateMeshRight, matrix_.view, matrix_.viewProj);
            }

            if(graphModule.drawMirror)
            {
                glm::vec3 translateMeshMirror=glm::vec3(-190.0f,33.0f,00.0f);
                graphModule.RenderMeshCut(graphModule.geo_.meshMirror,translateMeshMirror, matrix_.view, matrix_.viewProj);
            }
        }
        else {
            if(graphModule.drawHalf)
            {
                glm::vec3 translateMeshLeft=glm::vec3(-35.0f,33.0f,00.0f);
                graphModule.RenderMeshCut(graphModule.geo_.meshleft,translateMeshLeft, matrix_.view, matrix_.viewProj);
                glm::vec3 translateMeshRight=glm::vec3(35.0f,33.0f,00.0f);
                graphModule.RenderMeshCut(graphModule.geo_.meshright,translateMeshRight, matrix_.view, matrix_.viewProj);
            }

            if(graphModule.drawMirror)
            {
                glm::vec3 translateMeshMirror=glm::vec3(-190.0f,33.0f,00.0f);
                graphModule.RenderMeshCut(graphModule.geo_.meshMirror,translateMeshMirror, matrix_.view, matrix_.viewProj);
            }
        }

        if(graphModule.CreateNewMesh)
        {
            graphModule.CreateNewMesh=false;

            CreateNewMesh(deltatime_, interTick);

        }

        graphModule.GuiRender(originalMesh, planeOrigin,planeNormal, planeNormalRotate, angleOfRotationStep);
        SDL_GL_SwapWindow(graphModule.screen); //update window
        tick++;
    }

    graphModule.CloseRender();

    return true;
}


void MainLoop::configureMesh()
{
    originalMesh=MeshCreator::createSphere(1.0);
    //std::string objFile=path_string+"textures/"+ "torus.obj";

    //originalMesh = WavefrontObjSerializer::deserialize(objFile.c_str());
    originalMesh.recalculateVertices();
    graphModule.setup_mesh_geometry(graphModule.geo_.meshbase, originalMesh);

    slicer.attachMeshPtr(std::make_shared<MeshData>(originalMesh));
    slicer.sliceFace(planeOrigin,planeNormal,resultUpper,resultLower);

    graphModule.setup_mesh_geometry(graphModule.geo_.meshleft,  resultUpper);
    graphModule.setup_mesh_geometry(graphModule.geo_.meshright, resultLower);

    mirror.attachMeshPtr(std::make_shared<MeshData>(resultUpper));
    mirror.mirror(planeOrigin,planeNormal,resultMirror);
    graphModule.setup_mesh_geometry(graphModule.geo_.meshMirror, resultMirror);

}


void MainLoop::setup()
{
    //box.enabled = true;
    //axes.enabled = false;
    configureMesh();
   // glfwSetTime(time_);
}




void MainLoop::update_camera() {

  camera_.event(
    camera_event.bMouseMove, camera_event.bTranslatePressed, camera_event.bRotatePressed,
    camera_event.mouseX, camera_event.mouseY, camera_event.wheelDelta
  );

  /* Compute the view matrix */
  matrix_.view = glm::mat4(1.0f);
  matrix_.view = glm::lookAt(glm::vec3(0.0f, 0.65f*camera_.dolly(), camera_.dolly()),
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(0.0f, 1.0f, 0.0f));
  matrix_.view =glm::translate(matrix_.view, glm::vec3(camera_.translate_x(), camera_.translate_y(), 0.0f));
  matrix_.view = glm::rotate(
    glm::rotate(matrix_.view, camera_.yaw(), glm::vec3(1.0f, 0.0f, 0.0f)),
    camera_.pitch(), glm::vec3(0.0f, 1.0f, 0.0f)
  );

  /* Update the viewproj matrix */
  matrix_.viewProj = matrix_.proj * matrix_.view;
}

void MainLoop::update_time() {
  std::chrono::steady_clock::time_point tick = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(tick - time_);
  time_ = tick;

  deltatime_ = static_cast<float>(time_span.count());
}
