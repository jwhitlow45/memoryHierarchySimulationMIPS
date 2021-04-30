#include "Cache.hpp"

void Cache::setBlockHistory(int set, int blockToSet)
{
    sets[set][blockToSet].history = 1;
    sets[set][!blockToSet].history = 0;
}