#include "meshbuffer.h"
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include "vertexattributeindices.h"

MeshBuffer::MeshBuffer()
    : UsesNormals(false)
    , UsesUVs(false)
    , UsesIndices(false)
    , UsesGenerics(5, false)
    , VertCnt(0)
    , IdxCnt(0)
    , Generics(5)
{

}

MeshBuffer::~MeshBuffer()
{
    cleanUp();
}

MeshBuffer::MeshBuffer(const MeshBuffer & ref)
    : UsesNormals(false)
    , UsesUVs(false)
    , UsesIndices(false)
    , VertCnt(0)
    , IdxCnt(0)
{
    this->operator =(ref);
}

MeshBuffer& MeshBuffer::operator=(const MeshBuffer & ref)
{
    if (this == &ref) return *this;
    cleanUp();

    setVerts(ref.VertCnt, (float*)&ref.Verts[0]);
    setNorms(ref.VertCnt, (float*)&ref.Norms[0]);
    setTexCoords(0, ref.VertCnt, (float*)&ref.TexCoords[0]);

    for (size_t i=0; i<5; ++i)
        if (ref.UsesGenerics[i])
            setGenerics(i, ref.getGenerics(i));

    setIndices(ref.IdxCnt, (uint32_t*)&ref.Indices[0]);
    return *this;
}

void MeshBuffer::loadFile(const char * fileName)
{

}

void MeshBuffer::setVerts(unsigned int count, const float* verts)
{
    if (!verts) return;

    VertCnt = count;
    Verts.resize(VertCnt);
    int src_num_componets = 3;

    for (unsigned int i=0; i<VertCnt; ++i)
    {
        glm::vec3 vec;
        vec[0] = verts[i * src_num_componets + 0];
        vec[1] = verts[i * src_num_componets + 1];
        vec[2] = verts[i * src_num_componets + 2];
        Verts[i] = vec;
    }
}

void MeshBuffer::setNorms(unsigned int count, const float* normals)
{
    if (count != VertCnt)
    {
        std::cout << "Vert count does not match the number normals to create" << std::endl;
        exit(1);
    }

    if (!normals) return;
    UsesNormals = true;

    Norms.clear();
    Norms.resize(count);
    int src_num_componets = 3;

    for (unsigned int i=0; i<VertCnt; ++i)
    {
        glm::vec3 vec;
        vec[0] = normals[i * src_num_componets + 0];
        vec[1] = normals[i * src_num_componets + 1];
        vec[2] = normals[i * src_num_componets + 2];

        Norms[i] = vec;
    }
}

void MeshBuffer::setTexCoords(unsigned int layer, unsigned int count, const float* coords)
{
    if (count != VertCnt)
    {
        std::cout << "Vert count does not match the number uvs to create" << std::endl;
        exit(1);
    }

    if (!coords) return;
    UsesUVs = true;

    TexCoords.clear();
    TexCoords.resize(count);
    int src_num_componets = 2;

    for (unsigned int i=0; i<VertCnt; ++i)
    {
        glm::vec2 vec;
        vec[0] = coords[i * src_num_componets + 0];
        vec[1] = coords[i * src_num_componets + 1];

        TexCoords[i] = vec;
    }
}

void MeshBuffer::setGenerics(unsigned int index, const std::vector<glm::vec4>& values)
{
    if (values.size() != VertCnt)
    {
        std::cout << "Vert count does not match the number generic values to add to vbo" << std::endl;
        exit(1);
    }

    if (index >= (unsigned int)UsesGenerics.size())
    {
        std::cout << "setGenerics index is not within the valid range of [0-4]" << std::endl;
        exit(1);
    }
    UsesGenerics[index] = true;

    Generics[index].clear();
    Generics[index].resize(values.size());
    Generics[index] = values;
}

void MeshBuffer::setIndices(unsigned int count, const unsigned int * indices)
{
    if (!indices) return;
    UsesIndices = true;

    IdxCnt = count;
    Indices.clear();
    Indices.resize(IdxCnt);
    for (unsigned int i=0; i<IdxCnt; ++i)
    {
        Indices[i] = indices[i];
    }
}

const std::vector<glm::vec3>& MeshBuffer::getVerts() const
{
    return Verts;
}

const std::vector<glm::vec3>& MeshBuffer::getNorms() const
{
    return Norms;
}

const std::vector<glm::vec2>& MeshBuffer::getTexCoords(unsigned int layer) const
{
    return TexCoords;
}

const std::vector<glm::vec4>& MeshBuffer::getGenerics(unsigned int index) const
{
    return Generics[index];
}

const std::vector<uint32_t>& MeshBuffer::getIndices() const
{
    return Indices;
}

void MeshBuffer::generateFaceNormals()
{
    assert(IdxCnt);

    UsesNormals = true;
    Norms.clear();
    Norms.resize(VertCnt);
    for (unsigned int i=0; i<IdxCnt; i+=3){
        unsigned int idx_a = i+0;
        unsigned int idx_b = i+1;
        unsigned int idx_c = i+2;

        unsigned int vert_idx_a = Indices[idx_a];
        unsigned int vert_idx_b = Indices[idx_b];
        unsigned int vert_idx_c = Indices[idx_c];

        glm::vec3 vec_a = Verts[vert_idx_a];
        glm::vec3 vec_b = Verts[vert_idx_b];
        glm::vec3 vec_c = Verts[vert_idx_c];

        glm::vec3 edge_ab = vec_b - vec_a;
        glm::vec3 edge_ac = vec_c - vec_a;

        glm::vec3 norm = glm::normalize( glm::cross( edge_ab, edge_ac ) );
        Norms[vert_idx_a] = norm;
        Norms[vert_idx_b] = norm;
        Norms[vert_idx_c] = norm;
    }
}

unsigned int MeshBuffer::getVertCnt() const
{
    return VertCnt;
}

unsigned int MeshBuffer::getIdxCnt() const
{
    return IdxCnt;
}

void MeshBuffer::cleanUp()
{
    UsesNormals = false;
    UsesUVs     = false;
    UsesIndices = false;
    VertCnt     = 0;
    IdxCnt      = 0;

    Verts.clear();
    Norms.clear();
    TexCoords.clear();
    for (auto& gen: Generics)
        gen.clear();
    Generics.clear();

    Indices.clear();
}

