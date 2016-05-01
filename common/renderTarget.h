#ifndef ogle_framebuffer
#define ogle_framebuffer

#include <vector>

#include "texture.h"
#include "glm/glm.hpp"

namespace ogle
{
    struct RenderTarget
    {
        RenderTarget();

        void init(bool clearColor, bool clearDepth, bool clearStencil);
        void attachColor(const std::vector<Texture>& textures);
        void attachColor(const Texture& texture);
        void attachDepth(const Texture& texture);
        void attachStencil(const Texture& texture);
        void checkStatus();
        void bind();
        void unbind();
        void clear();
        void shutdown();

        unsigned int  FramebufferName;

        glm::vec4 ClearColor;
        float ClearDepth;
        float ClearStencil;

        bool UseClearColor;
        bool UseClearDepth;
        bool UseClearStencil;

        int NumColorBuffers;
    };
}
#endif // ogle_framebuffer