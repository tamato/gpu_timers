#include "path.h"

void Path::nearestPointIDs(uint32_t& curr, uint32_t& next, const glm::vec3& point)
{
    float min_distance = 9e23f;
    for (size_t i=0; i<Count; ++i){
        float len = glm::length(point - Points[i]);
        if (len < min_distance){
            min_distance = len;
            curr = i;
        }
    }

    next = (curr + 1) % Count;
}

glm::vec3&& Path::nearestTangent(const glm::vec3& point)
{
    return std::move(glm::vec3());
}

void Path::shutdown()
{
    Count = 0;
    Radii.clear();
    Points.clear();
}