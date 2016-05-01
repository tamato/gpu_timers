#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "meshbuffer.h"

namespace ogle {
class Renderable
{
public:
    Renderable();
    virtual ~Renderable();

    void init(const MeshBuffer& meshObj);

    void update();
    void render();
    void shutdown();

    void setMesh(const MeshBuffer& meshBuffer);

    unsigned int IndexRangeStart;
    unsigned int IndexRangeEnd;
    unsigned int IndiceCnt;

private:
    bool Dirty;                     // If dirty re calculate the mesh buffer to be drawn
    unsigned int VertCnt;
    std::vector<unsigned int> EnabledArrays;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int IBO;

    const unsigned int Normalidx;
    const unsigned int Color0idx;
    const unsigned int UVidx;

    unsigned int Stride;
    unsigned int NormOffset;
    unsigned int UvOffset;
    std::vector<unsigned int> GenericsOffsets;
    bool CleanedUp;
};
}

