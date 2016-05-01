#pragma once
#include <glm/glm.hpp>
#include "path.h"

namespace ogle
{
    class TorusGenerator
    {
    public:
        void centerlinePath(const Path& centerline);
        void silces(uint32_t count);
        void generate();

    protected:
        Path TorusCenterline;
        uint32_t Slices=16;

    public:    
        std::vector<glm::vec3> Positions;
        std::vector<glm::vec3> Normals;
        std::vector<uint32_t> Indices;
    };
}