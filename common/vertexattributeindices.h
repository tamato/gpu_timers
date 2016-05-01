#pragma once
namespace ogle {
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
    namespace VertexAttributeIndices {
        const int PositionIdx = 0;
        const int NormalIdx = 2;
        const int Generics[5] = {3,4,5,6,7};
        const int UVIdx = 8;
    };
}