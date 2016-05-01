#include "torusgenerator.h"

#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale,

using namespace ogle;

void TorusGenerator::centerlinePath(const Path& centerline)
{
    TorusCenterline = centerline;
}

void TorusGenerator::silces(uint32_t count)
{
    Slices = count;
}

void TorusGenerator::generate()
{
    std::vector<glm::vec3> base_pts(Slices);
    for (uint32_t i=0; i<Slices; ++i){
        float ang = i/float(Slices) * glm::two_pi<float>();
        base_pts[i] = glm::vec3( cos(ang), 0.f, sin(ang) );
    }

    std::vector<glm::vec3> averaged_tangent(TorusCenterline.Count);
    for (uint32_t i=0; i<TorusCenterline.Count; ++i){
        glm::vec3 prev_pt = TorusCenterline.Points[ (i-1 + TorusCenterline.Count) % TorusCenterline.Count ];
        glm::vec3 next_pt = TorusCenterline.Points[ (i+1) % TorusCenterline.Count ];

        glm::vec3 line = next_pt - prev_pt;
        glm::vec3 normalized_line = glm::normalize(line);
        averaged_tangent[i] = normalized_line;
    }
    const uint32_t pt_count = TorusCenterline.Count * Slices;
    Positions.resize(pt_count);
    Normals.resize(pt_count);

    const int32_t verts_per_quad = 6;
    Indices.resize(pt_count * verts_per_quad);
    
    for (uint32_t stack=0; stack<TorusCenterline.Count; ++stack){
        float angle = glm::dot(averaged_tangent[stack], glm::vec3(0,1,0));
        angle = acos(angle);
        glm::vec3 axis(0,0,1);
        if (stack != 0 && stack != (TorusCenterline.Count/2)) {
            axis = glm::cross(glm::vec3(0,1,0), averaged_tangent[stack]);
        }

        glm::mat4x4 transform;
        transform = glm::translate(transform, TorusCenterline.Points[stack]);
        transform = glm::scale(transform, glm::vec3(TorusCenterline.Radii[stack]));
        transform = glm::rotate(transform, angle, axis);

        uint32_t stack_id = (stack*Slices);
        for (uint32_t slice=0; slice < Slices; ++slice) {
            uint32_t id = stack_id + slice;
            glm::vec4 pos = transform * glm::vec4(base_pts[slice], 1.f);
            Positions[id] = glm::vec3(pos);
            Normals[id] =  glm::normalize(glm::vec3(pos) - TorusCenterline.Points[stack]);

            uint32_t vert = (stack_id*verts_per_quad) + (slice*verts_per_quad);

            // triangle 1
            Indices[vert+0] = (slice + stack_id) % pt_count;
            Indices[vert+1] = ((slice+1)%Slices + stack_id) % pt_count;
            Indices[vert+2] = (Indices[vert+1] + Slices) % pt_count;
         
            // triangle 2
            Indices[vert+3] = Indices[vert+0];
            Indices[vert+4] = Indices[vert+2];
            Indices[vert+5] = (Indices[vert+0] + Slices) % pt_count;
        }
    }

/*
    720-870-9297
    becky
*/
}
