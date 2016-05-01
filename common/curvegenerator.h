#pragma once
#include <glm/glm.hpp>
#include "path.h"

namespace ogle
{
    class CurveGenerator
    {
    public:
        // the size of points and radii must match exactly, and there must be at least 2
        void addLineSegments(const std::vector<glm::vec3>& points, const std::vector<float>& radii);
        void addLineSegments(const Path& line);
        void generate();

    protected:

    public:
        std::vector<Path> LineSegments;

        std::vector<glm::vec3> Positions;
        std::vector<glm::vec4> PrevPoint;
        std::vector<glm::vec4> NextPoint;
        std::vector<uint32_t> Indices;
    };
}
