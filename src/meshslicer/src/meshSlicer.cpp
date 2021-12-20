#include "../include/meshSlicer.h"

MeshSlicer::MeshSlicer (const std::shared_ptr<MeshData> meshData) :_meshData(meshData) {}

void MeshSlicer::sliceFace(const glm::vec3 & planeOrigin ,const glm::vec3 & planeNormal , MeshData & resultUpper , MeshData & resultLower ) {

    resultLower.ClearAll();
    resultUpper.ClearAll();

    MeshData resultUpperNew, resultLowerNew;

    // create a transformer to reference mesh to the referenc of the cutting plane located on XY plane
    glm::vec3 vectorUp (0.0f,0.0f,1.0f);
    PointTransformer transformer( vectorUp,planeNormal,planeOrigin );

    // create a new mesh to calculate intersections
    MeshData transformedMesh = *_meshData; // copy originalMesh
    transformedMesh.transformMesh([& transformer ](const glm::vec3 & v)->glm::vec3 {return transformer.transformOriginToTarget(v);});

    // the transformed mesh is transfomed so that the cutting plane is aligned with the XY plane.
    // polygons above this plane should be copied to the upper-mesh and polygons below to the resultLower
    // polygons that cross the XY plane are split in two

    //LinePlaneIntersectionCache lineIntersectionsCache;

    int numberOldIndices=_meshData->numberIndices();
    int numberNewLowerIndices=0;
    int numberNewUpperIndices=0;


    // evalute each polygon
    for ( auto & f: transformedMesh.triangelVertices) {

        // order indices of polygon from top to bottom regarding z value
        int top = 0;
        int middle = 0;
        int bottom=0;

        if (transformedMesh.vertices[f.one].z >= transformedMesh.vertices[f.two].z ) {
            if (transformedMesh.vertices[f.two].z >=  transformedMesh.vertices[f.three].z) {
                top = f.one;
                middle = f.two;
                bottom = f.three;
            } else if (transformedMesh.vertices[f.one].z >=  transformedMesh.vertices[f.three].z) {
                top = f.one;
                middle = f.three;
                bottom = f.two;
            } else {
                top = f.three;
                middle = f.one;
                bottom = f.two;
            }
        } else {
            if (transformedMesh.vertices[f.one].z >=  transformedMesh.vertices[f.three].z) {
                top = f.two;
                middle = f.one;
                bottom = f.three;
            } else if (transformedMesh.vertices[f.two].z >=  transformedMesh.vertices[f.three].z) {
                top = f.two;
                middle = f.three;
                bottom = f.one;
            } else {
                top = f.three;
                middle = f.two;
                bottom = f.one;
            }
        }
        // verify the ordering
        if(transformedMesh.vertices[top].z >= transformedMesh.vertices[middle].z  &&  transformedMesh.vertices[middle].z >= transformedMesh.vertices[bottom].z ) {
            // LOG_DEBUG("OK")
        } else {
            LOG_ERROR("Invalid top/middle/bottom");
        }

        // check if a split is needed
        if ( transformedMesh.vertices[bottom].z >= 0) {
            resultUpper.triangelVertices.push_back(f);
        }  else if ( transformedMesh.vertices[top].z <= 0) {
            resultLower.triangelVertices.push_back(f);
        }
        else {

            // SPLITING POLYGON
            glm::vec3 & transTopVertic = transformedMesh.vertices[top];
            glm::vec3 & transMiddleVertic = transformedMesh.vertices[middle];
            glm::vec3 & transBottomVertic = transformedMesh.vertices[bottom];

            glm::vec3 intersectMiddleToPlane,intersectTopToPlane;

            glm::vec3  tempPoint;
            float t=0;
            bool findIntersection= lineUtils.getIntersectionWithXYplane(transTopVertic,transBottomVertic,t,tempPoint);
            if(findIntersection)
            {
                intersectTopToPlane = transformer.transformTargetToOrigin(tempPoint);
            }
            else
            {
                printf("Something go wrong");
            }


            if (transformedMesh.vertices[middle].z >= 0) { // two on top

                glm::vec3  tempPoint;
                float t=0;
                bool findIntersection= lineUtils.getIntersectionWithXYplane(transMiddleVertic,transBottomVertic,t,tempPoint);
                if(findIntersection)
                {
                    intersectMiddleToPlane = transformer.transformTargetToOrigin(tempPoint);
                }
                else
                {
                    printf("Something go wrong");
                }

                //Lower face  indexTopIntersect, indexMiddleIntersect, bottom
                {
                    resultLowerNew.vertices.push_back(intersectTopToPlane);
                    int one=numberNewLowerIndices++;
                    resultLowerNew.vertices.push_back(intersectMiddleToPlane);
                    int two=numberNewLowerIndices++;
                    resultLowerNew.vertices.push_back(_meshData->vertices[bottom]);
                    int three=numberNewLowerIndices++;

                    TriangleVertices newTriangleBottom ( one, two, three );
                    resultLowerNew.triangelVertices.push_back(newTriangleBottom);
                }

                //Upper face   indexTopIntersect, middle,top
                {
                    resultUpperNew.vertices.push_back(intersectTopToPlane);
                    int one=numberNewUpperIndices++;
                    resultUpperNew.vertices.push_back(_meshData->vertices[middle]);
                    int two=numberNewUpperIndices++;
                    resultUpperNew.vertices.push_back(_meshData->vertices[top]);
                    int three=numberNewUpperIndices++;

                    TriangleVertices newTriangleBottom ( one, two, three );
                    resultUpperNew.triangelVertices.push_back(newTriangleBottom);
                }

                //Upper face   indexTopIntersect, middle,top
                {
                    resultUpperNew.vertices.push_back(intersectTopToPlane);
                    int one=numberNewUpperIndices++;
                    resultUpperNew.vertices.push_back(intersectMiddleToPlane);
                    int two=numberNewUpperIndices++;
                    resultUpperNew.vertices.push_back(_meshData->vertices[middle]);
                    int three=numberNewUpperIndices++;

                    TriangleVertices newTriangleBottom ( one, two, three );
                    resultUpperNew.triangelVertices.push_back(newTriangleBottom);
                }


            } else {
            // two below , one above

                glm::vec3  tempPoint;
                float t=0;
                bool findIntersection= lineUtils.getIntersectionWithXYplane(transTopVertic,transMiddleVertic,t,tempPoint);
                if(findIntersection)
                {
                    intersectMiddleToPlane = transformer.transformTargetToOrigin(tempPoint);
                }
                else
                {
                    printf("Something go wrong");
                }

                //Upper face  indexMiddleIntersect, indexTopIntersect , top
                {
                    resultUpperNew.vertices.push_back(intersectMiddleToPlane);
                    int one=numberNewUpperIndices++;
                    resultUpperNew.vertices.push_back(intersectTopToPlane);
                    int two=numberNewUpperIndices++;
                    resultUpperNew.vertices.push_back(_meshData->vertices[top]);
                    int three=numberNewUpperIndices++;

                    TriangleVertices newTriangleBottom ( one, two, three );
                    resultUpperNew.triangelVertices.push_back(newTriangleBottom);
                }

                //Lower face  indexTopIntersect, middle,bottom
                {
                    resultLowerNew.vertices.push_back(intersectTopToPlane);
                    int one=numberNewLowerIndices++;
                    resultLowerNew.vertices.push_back(_meshData->vertices[middle]);
                    int two=numberNewLowerIndices++;
                    resultLowerNew.vertices.push_back(_meshData->vertices[bottom]);
                    int three=numberNewLowerIndices++;

                    TriangleVertices newTriangleBottom ( one, two, three );
                    resultLowerNew.triangelVertices.push_back(newTriangleBottom);
                }

                //Lower face indexMiddleIntersect, indexTopIntersect ,middle
                {
                    resultLowerNew.vertices.push_back(intersectMiddleToPlane);
                    int one=numberNewLowerIndices++;
                    resultLowerNew.vertices.push_back(intersectTopToPlane);
                    int two=numberNewLowerIndices++;
                    resultLowerNew.vertices.push_back(_meshData->vertices[middle]);
                    int three=numberNewLowerIndices++;

                    TriangleVertices newTriangleBottom ( one, two, three );
                    resultLowerNew.triangelVertices.push_back(newTriangleBottom);
                }

            }

        }

    }

    resultLower.recalculateVertices(_meshData);
    resultUpper.recalculateVertices(_meshData);

    resultLower.addVertices(resultLowerNew);
    resultUpper.addVertices(resultUpperNew);


}

MeshMirror::MeshMirror(const std::shared_ptr<MeshData> meshData) :_meshData(meshData) {}

void MeshMirror::mirror(const glm::vec3 &planeOrigin, const glm::vec3 &planeNormal, MeshData &resultMirror)
{
    MeshData resultUpperNew, resultLowerNew;

    // create a transformer to reference mesh to the referenc of the cutting plane located on XY plane
    glm::vec3 vectorUp (0.0f,0.0f,1.0f);
    PointTransformer transformer( vectorUp,planeNormal,planeOrigin );

    // create a new mesh to calculate intersections
    MeshData transformedMesh = *_meshData; // copy originalMesh
    transformedMesh.transformMesh([& transformer ](const glm::vec3 & v)->glm::vec3 {return transformer.transformOriginToTarget(v);});

    transformedMesh.transformMesh([& transformer ](const glm::vec3 & v)->glm::vec3 {return transformer.mirrorZ(v);});

    transformedMesh.transformMesh([& transformer ](const glm::vec3 & v)->glm::vec3 {return transformer.transformTargetToOrigin(v);});

    resultMirror=transformedMesh;

    //mirror add old


    resultMirror.addVertices(*_meshData);




};
