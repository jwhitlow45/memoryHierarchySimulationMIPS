#include "RegFile.hpp"

int RegFile::getReg(int regNum)
{
    if (regNum == 0)
        return zero;
    return reg[regNum - 16];
}

void RegFile::setReg(int regNum, int value)
{
    reg[regNum - 16] = value;
}