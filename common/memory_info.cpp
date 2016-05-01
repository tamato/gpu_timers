#include "memory_info.h"

/*

const GLubyte* sExtensions = glGetString(GL_EXTENSIONS);
GetIntegerv


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