#include <stdlib.h>
#include <iostream>

#include <glad/glad.h>

#include "renderable.h"

#include "vertexattributeindices.h"

using namespace ogle;

#define bufferOffest(x) ((char*)NULL+(x))

Renderable::Renderable()
    : IndiceCnt(0)
    , VertCnt(0)
    , VAO(0)
    , VBO(0)
    , IBO(0)
    , IndexRangeStart(0)
    , IndexRangeEnd(0)
    , Normalidx(2)
    , Color0idx(3)
    , UVidx(8)
    , CleanedUp(false)
{
    /************************************************************************************
      According to:
      http://www.opengl.org/registry/specs/NV/vertex_program.txt

      vertex attribute indices are:

        Vertex
        Attribute  Conventional                                           Conventional
        Register   Per-vertex        Conventional                         Component
        Number     Parameter         Per-vertex Parameter Command         Mapping
        ---------  ---------------   -----------------------------------  ------------
         0         vertex position   Vertex                               x,y,z,w
         1         vertex weights    VertexWeightEXT                      w,0,0,1
         2         normal            Normal                               x,y,z,1
         3         primary color     Color                                r,g,b,a
         4         secondary color   SecondaryColorEXT                    r,g,b,1
         5         fog coordinate    FogCoordEXT                          fc,0,0,1
         6         -                 -                                    -
         7         -                 -                                    -
         8         texture coord 0   MultiTexCoord(GL_TEXTURE0_ARB, ...)  s,t,r,q
         9         texture coord 1   MultiTexCoord(GL_TEXTURE1_ARB, ...)  s,t,r,q
         10        texture coord 2   MultiTexCoord(GL_TEXTURE2_ARB, ...)  s,t,r,q
         11        texture coord 3   MultiTexCoord(GL_TEXTURE3_ARB, ...)  s,t,r,q
         12        texture coord 4   MultiTexCoord(GL_TEXTURE4_ARB, ...)  s,t,r,q
         13        texture coord 5   MultiTexCoord(GL_TEXTURE5_ARB, ...)  s,t,r,q
         14        texture coord 6   MultiTexCoord(GL_TEXTURE6_ARB, ...)  s,t,r,q
         15        texture coord 7   MultiTexCoord(GL_TEXTURE7_ARB, ...)  s,t,r,q

        Table X.2:  Aliasing of vertex attributes with conventional per-vertex
        parameters.
    **************************************************************************************/
}

Renderable::~Renderable()
{
    shutdown();
}

void Renderable::init(const MeshBuffer& meshObj)
{
    // setup the mesh 
    setMesh(meshObj);
    CleanedUp = false;
}

void Renderable::update()
{

}

void Renderable::render()
{
    glBindVertexArray(VAO);

    if (IndiceCnt)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, IndexRangeEnd, GL_UNSIGNED_INT, (const void*)(IndexRangeStart * sizeof(unsigned int)));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, VertCnt);
    }

    glBindVertexArray(0);
}

void Renderable::shutdown()
{
    if (CleanedUp)
        return;

    for (const auto& earray : EnabledArrays)
        glDisableVertexAttribArray(earray);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteBuffers(1, &IBO);
    glDeleteVertexArrays(1, &VAO);
    CleanedUp = true;
}

void Renderable::setMesh(const MeshBuffer& meshBuffer)
{
    VertCnt = meshBuffer.getVertCnt();

    unsigned int vertArrayCnt = VertCnt;
    int VertComponentCount = sizeof(glm::vec3) / sizeof(glm::vec3::value_type);
    int NormalComponentCount = sizeof(glm::vec3) / sizeof(glm::vec3::value_type);
    int UVComponentCount = sizeof(glm::vec2) / sizeof(glm::vec2::value_type);
    int GenericComponentCount = sizeof(glm::vec4) / sizeof(glm::vec4::value_type);
    Stride = VertComponentCount; // will always have positions

    NormOffset = 0;
    UvOffset = 0;
    GenericsOffsets.clear();
    std::vector<std::vector<glm::vec4>> generics;

    EnabledArrays.clear();
    EnabledArrays.push_back(0); // 0 is for positions
    if (meshBuffer.UsesNormals)
    {
        EnabledArrays.push_back(Normalidx);
        vertArrayCnt += VertCnt;
        NormOffset = Stride;
        Stride += NormalComponentCount;
    }
    if (meshBuffer.UsesUVs)
    {
        EnabledArrays.push_back(UVidx);
        vertArrayCnt += VertCnt;
        UvOffset = Stride;
        Stride += UVComponentCount;
    }

    for (unsigned int i = 0; i < meshBuffer.UsesGenerics.size(); i++)
    {
        if (meshBuffer.UsesGenerics[i])
        {
            EnabledArrays.push_back(VertexAttributeIndices::Generics[i]);
            vertArrayCnt += VertCnt;
            GenericsOffsets.push_back(Stride);
            Stride += GenericComponentCount;

            generics.push_back(meshBuffer.getGenerics(i));
        }
    }

    const std::vector<glm::vec3>& pos = meshBuffer.getVerts();
    const std::vector<glm::vec3>& norm = meshBuffer.getNorms();
    const std::vector<glm::vec2>& uv = meshBuffer.getTexCoords(0);

    float* vertArray = new float[VertCnt*Stride];

    for (int i=0; i<VertCnt; ++i)
    {
        int vi = i*Stride;
        vertArray[vi+0] = pos[i][0];
        vertArray[vi+1] = pos[i][1];
        vertArray[vi+2] = pos[i][2];

        if (NormOffset)
        {
            int ni = i*Stride+NormOffset;
            vertArray[ni+0] = norm[i][0];
            vertArray[ni+1] = norm[i][1];
            vertArray[ni+2] = norm[i][2];
        }

        if (UvOffset)
        {
            int ti = i*Stride+UvOffset;
            vertArray[ti+0] = uv[i][0];
            vertArray[ti+1] = uv[i][1];
        }

        for (int g=0; g<(int)generics.size(); ++g)
        {
            int gi = i*Stride+GenericsOffsets[g];
            vertArray[gi+0] = generics[g][i][0];
            vertArray[gi+1] = generics[g][i][1];
            vertArray[gi+2] = generics[g][i][2];
            vertArray[gi+3] = generics[g][i][3];
        }
    }

    // make values go from number of componets to number of bytes
    Stride *= 4;
    NormOffset *= 4;
    UvOffset *= 4;
    for (int g=0; g<(int)GenericsOffsets.size(); ++g)
        GenericsOffsets[g] *= 4;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    for (const auto& earray : EnabledArrays)
        glEnableVertexAttribArray(earray);

    // set vert buffer    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 
        VertCnt * Stride,
        (GLvoid*)vertArray,
        GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Stride, bufferOffest(0));

    if (NormOffset)
        glVertexAttribPointer(Normalidx, 3, GL_FLOAT, GL_FALSE, Stride, bufferOffest(NormOffset));

    if (UvOffset)
        glVertexAttribPointer(UVidx, 2, GL_FLOAT, GL_FALSE, Stride, bufferOffest(UvOffset));
    
    for (int g=0; g<(int)GenericsOffsets.size(); ++g){
        glVertexAttribPointer(VertexAttributeIndices::Generics[g], 4, GL_FLOAT, GL_FALSE, Stride, bufferOffest(GenericsOffsets[g]));
    }

    delete [] vertArray;

    IndiceCnt = meshBuffer.getIdxCnt();
    if (IndiceCnt)
    {        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            IndiceCnt * sizeof(GLuint),
            (GLvoid*)meshBuffer.getIndices().data(),
            GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        IndexRangeStart = 0;
        IndexRangeEnd = IndiceCnt;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

