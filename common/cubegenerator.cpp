#include "cubegenerator.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace ogle;

CubeGenerator::CubeGenerator()
    : Stacks(2)
    , Slices(2)
    , Scale(1)
{

}

void CubeGenerator::scale(float scale)
{
    Scale = scale;
}

void CubeGenerator::tessellation_density(unsigned int stacks, unsigned int slices)
{
    if (stacks < 2) stacks = 2;
    if (slices < 2) slices = 2;
    Stacks = stacks;
    Slices = slices;
}

void CubeGenerator::tessellation_density(unsigned int density)
{
    if (density < 2) density = 2;
    Stacks = Slices = density;
}

void CubeGenerator::generate()
{
    Positions.clear();
    Positions.resize(Stacks*Slices*6); // 6  == sides to a cube

    Normals.clear();
    Normals.resize(Stacks*Slices*6);

    TexCoords.clear();
    TexCoords.resize(Stacks*Slices*6);

    float scale = Scale;

    // generate a plane that the verts will be based off of
    vector<glm::vec2> plane(Stacks*Slices);
    vector<glm::vec2> uvs(Stacks*Slices);
    for (unsigned int v=0; v<Stacks; ++v) {
        for (unsigned int u=0; u<Slices; ++u) {
            float x = scale * (u / float(Slices-1) - 0.5f);
            float y = scale * (v / float(Stacks-1) - 0.5f);
            plane[u + v*Slices] = glm::vec2(x,y);

            float s = u / float(Slices-1);
            float t = v / float(Stacks-1);
            uvs[u + v*Slices] = glm::vec2(s,t);
        }
    }

    // z faces
    unsigned int offset0 = 0;
    unsigned int offset1 = Stacks*Slices;
    for (unsigned int v=0; v<Stacks; ++v) {
        for (unsigned int u=0; u<Slices; ++u) {
            float x = plane[u + v*Slices][0];
            float y = plane[u + v*Slices][1];
            float z = scale * .5f;

            Positions[offset0 + u + v*Slices] = glm::vec3( x, y, z);
            Normals  [offset0 + u + v*Slices] = glm::vec3( 0, 0, 1);
            TexCoords[offset0 + u + v*Slices] = uvs[u + v*Slices];
            
            Positions[offset1 + u + v*Slices] = glm::vec3(-x, y,-z);
            Normals  [offset1 + u + v*Slices] = glm::vec3( 0, 0,-1);
            TexCoords[offset1 + u + v*Slices] = uvs[u + v*Slices];
        }
    }

    // x faces
    offset0 += Stacks*Slices*2;
    offset1 += Stacks*Slices*2;
    for (unsigned int v=0; v<Stacks; ++v) {
        for (unsigned int u=0; u<Slices; ++u) {
            float x = scale * .5f;
            float y = plane[u + v*Slices][0];
            float z = plane[u + v*Slices][1];

            Positions[offset0 + u + v*Slices] = glm::vec3(-x, y, z);
            Normals  [offset0 + u + v*Slices] = glm::vec3(-1, 0, 0);
            TexCoords[offset0 + u + v*Slices] = uvs[u + v*Slices];
            
            Positions[offset1 + u + v*Slices] = glm::vec3( x, y,-z);
            Normals  [offset1 + u + v*Slices] = glm::vec3( 1, 0, 0);
            TexCoords[offset1 + u + v*Slices] = uvs[u + v*Slices];
        }
    }

    // y faces
    offset0 += Stacks*Slices*2;
    offset1 += Stacks*Slices*2;
    for (unsigned int v=0; v<Stacks; ++v) {
        for (unsigned int u=0; u<Slices; ++u) {
            float x = plane[u + v*Slices][0];
            float y = scale * .5f;
            float z = plane[u + v*Slices][1];

            Positions[offset0 + u + v*Slices] = glm::vec3(x, y,-z);
            Normals  [offset0 + u + v*Slices] = glm::vec3(0, 1, 0);
            TexCoords[offset0 + u + v*Slices] = uvs[u + v*Slices];
            
            Positions[offset1 + u + v*Slices] = glm::vec3(x,-y, z);
            Normals  [offset1 + u + v*Slices] = glm::vec3(0,-1, 0);
            TexCoords[offset1 + u + v*Slices] = uvs[u + v*Slices];
        }
    }

    // 6 faces to a cube, 2 triangles each face, 3 verts per triangle
    Indices.clear();
    Indices.resize( (Stacks-1)*(Slices-1)*6*2*3);
    unsigned int counter = 0;
    for (unsigned int k=0; k<6; ++k){
        for (unsigned int j=0; j<Stacks-1; ++j){
            for (unsigned int i=0; i<Slices-1; ++i) {
                unsigned int idx = i + j*Slices + k*Stacks*Slices;
                Indices[counter + 0] = idx+0;
                Indices[counter + 1] = idx+1;
                Indices[counter + 2] = idx+Slices;

                Indices[counter + 3] = idx+Slices;
                Indices[counter + 4] = idx+1;
                Indices[counter + 5] = idx+Slices+1;

                counter += 6;
            }
        }
    }
}
