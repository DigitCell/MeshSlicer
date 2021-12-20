#pragma once

#include <fstream>
#include <ostream>
#include <iostream>

#include "log.h"
#include "meshData.h"


#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

class WavefrontObjSerializer {

public:   

    static void serialize( const char* fileName, MeshData & meshData ) {
        std::ofstream myfile;

        myfile.open (fileName);

            myfile << "# OBJ file format with ext .obj" << std::endl;
            myfile << "# vertex count = " << std::to_string(meshData.vertices.size()) <<std::endl; 
            myfile << "# face count = " << std::to_string(meshData.triangelVertices.size()) << std::endl;
     
            for (auto & v : meshData.vertices) {
                myfile <<"v " <<std::scientific << v.x << " " << v.y << " " << v.z << std::endl;
            }
            for (auto & v : meshData.triangelVertices) {
                myfile <<"f " << v.one+1 << " " << v.two+1 << " " << v.three+1<< std::endl;
            }

        myfile.close();
    }


    static MeshData deserialize(const char* fileName)
    {
        MeshData data;

        data.vertices.clear();
        data.triangelVertices.clear();

        FILE* file = fopen(fileName, "r");
        if (file != NULL) {
            while (1) {
                // todo make safe for bad format of OBJ file
                char lineHeader[128];
                // read the first word of the line
                int res = fscanf(file, "%s", lineHeader);
                if (res == EOF)
                    break; // EOF = End Of File. Quit the loop.                          

                if (strcmp(lineHeader, "v") == 0) {
                    glm::vec3 vertex;
                    fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                    data.vertices.push_back(vertex);
                    //LOG_DEBUG("vertices:" + glm::to_string(vertex));

                } else if (strcmp(lineHeader, "f") == 0) {
                    TriangleVertices vertices;
                    fscanf(file, "%i %i %i\n", &vertices.one, &vertices.two, &vertices.three);
                    vertices.one-=1;
                    vertices.two-=1;
                    vertices.three-=1;
                    data.triangelVertices.push_back(vertices);
                    //LOG_DEBUG("triangles:" + std::to_string(vertices.one));
                }
            }
        } 
        else {
            //todo
        }
        return data;
    }
};
