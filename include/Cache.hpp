#pragma once
#include "Block.hpp"
#include <cstddef>

class Cache
{
public:
    Block sets[8][2];

    void setBlockHistory(int, int); //set history bits based on lru
    void setBlockValid(int, int);   //set block as valid
    int getVictimBlock(int);        //return mose recent block
};