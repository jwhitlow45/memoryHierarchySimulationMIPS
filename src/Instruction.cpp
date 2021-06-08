#include "Instruction.hpp"
#include <math.h>
#include <iostream>

using namespace std;

int Instruction::posBinaryToInt(string binary)
{
    int total = 0;
    for (size_t i = 0; i < binary.size(); i++)
        total += (binary[i] - '0') * pow(2, binary.size() - i - 1);
    return total;
}

void Instruction::decodeInstruction(string binInst)
{
    //decode binary instruction and set all values
    binaryInst = binInst;
    opcode = posBinaryToInt(binInst.substr(0, 6));
    baseReg = posBinaryToInt(binInst.substr(6, 5));
    offset = posBinaryToInt(binInst.substr(16, 16));
    targetReg = posBinaryToInt(binInst.substr(11, 5));
    byteAddress = baseReg + offset;
    wordAddress = byteAddress / 4;
}

void Instruction::print()
{
    cout << opcode << ' ' << baseReg << ' ' << targetReg << ' ' << offset;
}

void Instruction::printBinary()
{
    cout << binaryInst;
}

void Instruction::printCacheResult()
{
    if (hit)
    {
        if (opcode == LW)
            cout << "read_hit";
        else if (opcode == SW)
            cout << "write_hit";
    }
    else
    {
        if (opcode == LW)
            cout << "read_miss";
        else if (opcode == SW)
            cout << "write_miss";
    }
}