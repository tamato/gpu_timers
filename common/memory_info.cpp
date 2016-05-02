/*
http://developer.download.nvidia.com/opengl/specs/GL_NVX_gpu_memory_info.txt
GL_NVX_gpu_memory_info 


GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX 
  - dedicated video memory, total size (in kb) of the GPU memory

GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX 
  - total available memory, total size (in Kb) of the memory 
    available for allocations

GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 
  - current available dedicated video memory (in kb), 
    currently unused GPU memory

GPU_MEMORY_INFO_EVICTION_COUNT_NVX 
  - count of total evictions seen by system

GPU_MEMORY_INFO_EVICTED_MEMORY_NVX 
  - size of total video memory evicted (in kb)




https://www.opengl.org/registry/specs/ATI/meminfo.txt
GL_ATI_meminfo

The query returns a 4-tuple
    integer where the values are in Kbyte and have the following meanings:

      param[0] - total memory free in the pool
      param[1] - largest available free block in the pool
      param[2] - total auxiliary memory free
      param[3] - largest auxiliary free block

    These values are only useful as a hint as the implementation cannot
    guarantee the lack of allocations between commands, and the
    information may have changed before the attempt to use it. AUXILIARY
    memory is memory that an implementation may use as a backup to its
    primary pool for a certain type of allocation. The pools are not
    guaranteed to be either independent or shared, as this is an
    implementation choice. 

GL_VBO_FREE_MEMORY_ATI
GL_TEXTURE_FREE_MEMORY_ATI
GL_RENDERBUFFER_FREE_MEMORY_ATI

*/

#include "memory_info.h"

#include <cstring>
#include <functional>
#include <vector>
#include <iostream>

#include <glad/glad.h>

#include "checkextensions.h"

namespace ogle
{
    namespace 
    {
        MemoryLimits limits_not_supported()
        {
            MemoryLimits result = {-1,-1,-1,-1,-1,-1,-1};
            return result;
        }

        MemoryLimits limits_nvidia()
        {
            MemoryLimits result = {-1,-1,-1,-1,-1,-1,-1};

            GLint kb;
            glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &kb);
            result.total_video = kb;
            glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &kb);
            result.total_system = kb;
            glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &kb);
            result.gpu_available = kb;
            return result;
        }

        MemoryLimits limits_ati()
        {
            MemoryLimits result = {-1,-1,-1,-1,-1,-1,-1};
            GLint kb[4];
            glGetIntegerv(GL_VBO_FREE_MEMORY_ATI, kb);
            result.total_pool       = kb[0];
            result.largest_block    = kb[1];
            result.total_aux        = kb[2];
            result.largest_aux      = kb[3];
            return result;
        }

        MemoryEvictionInfo eviction_not_supported()
        {
            MemoryEvictionInfo result = {-1,-1};
            return result;
        }

        MemoryEvictionInfo eviction_nvidia()
        {
            MemoryEvictionInfo result = {-1,-1};
            
            GLint kb;
            glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX, &kb);
            result.count = kb;
            glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &kb);
            result.size = kb;
            return result;
        }

        std::function<MemoryLimits ()> limits_func;
        std::function<MemoryEvictionInfo ()> evict_func;
    }

    void memory_info_init()
    {
        // check for specific GL version to know which style of extension checking to do
        //std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
        limits_func = limits_not_supported;
        evict_func = eviction_not_supported;

        std::vector<std::string> exts;
        exts.push_back("GL_NVX_gpu_memory_info");
        exts.push_back("GL_ATI_meminfo");
        
        std::vector<bool> supported = ogle::extensions_supported(exts);
        
        if (supported[0]){
            limits_func = limits_nvidia;
            evict_func = eviction_nvidia;
        }
        else if (supported[1]){
            limits_func = limits_ati;
        }
    }

    MemoryLimits memory_info_report_limits()
    {
        return limits_func();
    }

    void memory_info_print(const MemoryLimits& ml)
    {
        std::cout << "total_video: " << ml.total_video << std::endl;
        std::cout << "total_system: " << ml.total_system << std::endl;
        std::cout << "gpu_available: " << ml.gpu_available << std::endl;

        std::cout << "total_pool: " << ml.total_pool << std::endl;
        std::cout << "largest_block: " << ml.largest_block << std::endl;
        std::cout << "total_aux: " << ml.total_aux << std::endl;
        std::cout << "largest_aux: " << ml.largest_aux << std::endl;
    }

    MemoryEvictionInfo memory_info_report_eviction()
    {
        return evict_func();
    }

    void memory_info_print(const MemoryEvictionInfo& mei)
    {
        std::cout << "count: " << mei.count << std::endl;
        std::cout << "size: " << mei.size << std::endl;
    }

    void memory_info_print(const MemoryLimits& ml, const MemoryEvictionInfo& mei)
    {
        if (ml.total_pool == -1 && ml.total_video > -1)
            std::cout << "TV: " << ml.total_video << " TS: " << ml.total_system << " GPU: " << ml.gpu_available << " EC: " << mei.count << " ES: " << mei.size << std::endl;
        
        if (ml.total_pool > -1 && ml.total_video == -1)
            std::cout << "TP: " << ml.total_pool << " LB: " << ml.largest_block << " TA: " << ml.total_aux << " LA: " << ml.largest_aux << std::endl;
    }
}