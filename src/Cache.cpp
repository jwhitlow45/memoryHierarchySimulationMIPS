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
    //check if set 0 is valid, return set 0
    if(!sets[setNumber][0].valid)
        return 0;
    //check if set 1 is valid, return set 1
    if(!sets[setNumber][1].valid)
        return 1;
    //if both are valid check history bit to select victim
    if (sets[setNumber][0].history == 1)
        return 1;
    return 0;
}