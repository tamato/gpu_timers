/*
    Simple class to help with testing memory limits
*/
#pragma once

#include <stdint.h> // int32_t

namespace ogle
{
    // a value of -1 means it is not supported or not set
    struct MemoryEvictionInfo
    {
        int32_t count;  // count of total evictions seen by system
        int32_t size;   // size of total video memory evicted (in kb)
    };

    // a value of -1 means it is not supported or not set
    struct MemoryLimits
    {
        // Nvidia
        int32_t total_video;    // dedicated video memory, total size (in kb) of the GPU memory
        int32_t total_system;   // total available memory, total size (in Kb) of the memory available for allocations
        int32_t gpu_available;  // current available dedicated video memory (in kb), currently unused GPU memory

        // Ati
        int32_t total_pool;     // total memory free in the pool
        int32_t largest_block;  // largest available free block in the pool
        int32_t total_aux;      // total auxiliary memory free
        int32_t largest_aux;    // largest auxiliary free block
    };

    // find which (if any required extensions are supported, a OpenGL context MUST be current before calling this)
    void memory_info_init();

    MemoryLimits memory_info_report_limits();
    void memory_info_print(const MemoryLimits& ml);

    MemoryEvictionInfo memory_info_report_eviction();
    void memory_info_print(const MemoryEvictionInfo& mei);

    void memory_info_print(const MemoryLimits& ml, const MemoryEvictionInfo& mei);
}
