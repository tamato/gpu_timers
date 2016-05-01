#pragma once

#include <unordered_map>
#include <vector>
#include <stdint.h>

namespace ogle {

class SpatialHash
{
public:
private:
    typedef size_t hash_value;

    uint32_t CellSize;
    std::unordered_map<hash_value, std::vector<uint32_t> > Objects;
};

}
