#pragma once

#include <vector>
#include "glm/glm.hpp"

struct Path
{
public:
    std::vector<float> Radii;
    std::vector<glm::vec3> Points;
    uint32_t Count;

    void nearestPointIDs(uint32_t& curr, uint32_t& next, const glm::vec3& point);
    glm::vec3&& nearestTangent(const glm::vec3& point);
    void shutdown();
};