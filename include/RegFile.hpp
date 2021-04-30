#pragma once
#define NUMREG 8

#include<cstddef>

class RegFile
{
public:
    int reg[NUMREG];
    const int zero = 0;

    RegFile()
    {
        for (size_t i = 0; i < NUMREG; i++)
            reg[i] = 0;
    }

    int getReg(int);
    void setReg(int, int);
};