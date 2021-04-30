#pragma once
#define NUMREG 8

#include<cstddef>

class RegFile
{
public:
    int reg[NUMREG];

    RegFile()
    {
        for (size_t i = 0; i < NUMREG; i++)
            reg[i] = 0;
    }
};