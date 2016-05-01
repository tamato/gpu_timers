#ifndef MESH_OBJECT_H_
#define MESH_OBJECT_H_

#include "glm/glm.hpp"
#include "meshbuffer.h"

class MeshObject
{
public:
    MeshObject();
    virtual ~MeshObject();

    void init(const MeshBuffer& meshObj);

    void update();
    void render();
    void shutdown();

    void setMesh(const MeshBuffer& meshBuffer);

    void computeBoundingBox();

    const MeshBuffer& getMesh();

    glm::vec3 PivotPoint;
    glm::vec3 AABBMin;
    glm::vec3 AABBMax;

    unsigned int IndexRangeStart;
    unsigned int IndexRangeEnd;
    unsigned int IndiceCnt;

private:
    bool Dirty;                     // If dirty re calculate the mesh buffer to be drawn
    unsigned int VertCnt;
    unsigned int EnabledArrays;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int IBO;

    MeshBuffer Mesh;

    unsigned int Normalidx;
    unsigned int UVidx;
    unsigned int Color0idx;
    unsigned int Color1idx;
    
    unsigned int Stride;
    
    unsigned int NormOffset;
    unsigned int UvOffset;
    unsigned int Color0Offset;
    unsigned int Color1Offset;

    bool CleanedUp;
};

#endif // MESH_OBJECT_H_
