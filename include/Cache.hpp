#pragma once
#include "Block.hpp"

class Cache
{
public:
    Block sets[8][2];

    void setBlockHistory(int, int); //set history bits based on lru
};