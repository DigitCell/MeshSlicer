#pragma once

#include <memory>

#include <glm/glm.hpp>
#include "log.h"
#include "meshData.h"
#include "pointTransformer.h"
#include "utils.h"
#include "linePlaneIntersectionCache.h"


class MeshSlicer {
    public :
        MeshSlicer() {};
        MeshSlicer (const std::shared_ptr<MeshData> meshData) ;

        void attachMeshPtr(const std::shared_ptr<MeshData> meshData){ _meshData=meshData;};

        void sliceFace(const glm::vec3 &planeOrigin, const glm::vec3 &planeNormal, MeshData &resultUpper, MeshData &resultLower);
private:
        LineUtils lineUtils;
        std::shared_ptr<MeshData> _meshData;
};


class MeshMirror {
    public :
        MeshMirror() {};
        MeshMirror (const std::shared_ptr<MeshData> meshData) ;

        void attachMeshPtr(const std::shared_ptr<MeshData> meshData){ _meshData=meshData;};

        void mirror(const glm::vec3 &planeOrigin, const glm::vec3 &planeNormal, MeshData &resultMirror);
private:
        LineUtils lineUtils;
        std::shared_ptr<MeshData> _meshData;
};
