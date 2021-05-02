#pragma once
#define MEMSIZE 128

#include <cstddef>

class Memory
{
public:
    int memory[MEMSIZE];

    Memory()
    {
        for (size_t i = 0; i < MEMSIZE; i++) // init memory to mem location + 5
            memory[MEMSIZE] = MEMSIZE + 5;
    }

    void printMemory();
};