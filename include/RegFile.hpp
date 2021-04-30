#pragma once
#define NUMREG 8

#include <cstddef>

class RegFile
{
public:
    int reg[NUMREG];    //registers s0-s7
    const int zero = 0; //zero register

    RegFile()
    {
        for (size_t i = 0; i < NUMREG; i++)
            reg[i] = 0;
    }

    int getReg(int);       //return register given register number
    void setReg(int, int); //set register given register number and value
};