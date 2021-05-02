#include "Cache.hpp"

void Cache::setBlockHistory(int set, int blockToSet)
{
    sets[set][blockToSet].history = 1;
    sets[set][!blockToSet].history = 0;
}

void Cache::setBlockValid(int set, int blockToSet)
{
    sets[set][blockToSet].valid = 1;
}

int Cache::getVictimBlock(int setNumber)
{
    if (sets[setNumber][0].history == 1)
        return 1;
    return 0;
}