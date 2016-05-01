#include "curvegenerator.h"

#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale,

using namespace ogle;

void CurveGenerator::addLineSegments(const std::vector<glm::vec3>& points, const std::vector<float>& radii)
{

}

void CurveGenerator::addLineSegments(const Path& line)
{
    LineSegments.push_back( line );
}

void CurveGenerator::generate()
{
    // blindly make quads that are orientated to the Z axis, they'll get updated in the vert shader to be alined to the camera's view vector

    const std::vector<glm::vec3>& point_list = LineSegments[0].Points;
    const std::vector<float>& radiusList = LineSegments[0].Radii;

    const uint32_t ptls_size = point_list.size();
    const uint32_t pts_per_line = 3;
    const uint32_t pt_count = ptls_size * pts_per_line;
    const uint32_t verts_per_strip = 12;

    Indices.resize((ptls_size-1) * verts_per_strip);
    Positions.resize(pt_count);
    PrevPoint.resize(pt_count);
    NextPoint.resize(pt_count);
    for (uint32_t idx =0; idx<ptls_size; ++idx){

        // generate our normal for placing vert positions
        uint32_t line_idx = idx;
        if (idx == 0) line_idx = 1;
        if (idx == ptls_size-1) line_idx = ptls_size-2;

        // fill out vert positions
        uint32_t pt_idx = idx * pts_per_line;
        Positions[pt_idx+0] = point_list[idx];
        PrevPoint[pt_idx+0] = glm::vec4(point_list[line_idx-1], +radiusList[idx]);
        NextPoint[pt_idx+0] = glm::vec4(point_list[line_idx+1], +radiusList[idx]);

        Positions[pt_idx+1] = point_list[idx];
        PrevPoint[pt_idx+1] = glm::vec4(point_list[line_idx-1], 0);
        NextPoint[pt_idx+1] = glm::vec4(point_list[line_idx+1], 0);

        Positions[pt_idx+2] = point_list[idx];
        PrevPoint[pt_idx+2] = glm::vec4(point_list[line_idx-1], -radiusList[idx]);
        NextPoint[pt_idx+2] = glm::vec4(point_list[line_idx+1], -radiusList[idx]);

        // fill out indices that will make our quads that make up the line
        uint32_t vert_idx = idx * verts_per_strip;
        
        if (idx == ptls_size-1) continue;
        // left edge
        Indices[vert_idx+ 0] = 0 + pt_idx;
        Indices[vert_idx+ 1] = 1 + pt_idx;
        Indices[vert_idx+ 2] = 4 + pt_idx;

        Indices[vert_idx+ 3] = 0 + pt_idx;
        Indices[vert_idx+ 4] = 4 + pt_idx;
        Indices[vert_idx+ 5] = 3 + pt_idx;

        // right edge
        Indices[vert_idx+6] = 1 + pt_idx;
        Indices[vert_idx+7] = 2 + pt_idx;
        Indices[vert_idx+8] = 5 + pt_idx;

        Indices[vert_idx+9] = 1 + pt_idx;
        Indices[vert_idx+10] = 5 + pt_idx;
        Indices[vert_idx+11] = 4 + pt_idx;
    }
}
