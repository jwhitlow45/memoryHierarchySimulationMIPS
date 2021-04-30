#include "Instruction.hpp"
#include <math.h>

int Instruction::posBinaryToInt(string binary)
{
    int total = 0;
    for (size_t i = 0; i < binary.size(); i++)
        total += (binary[i] - '0') * pow(2, binary.size() - i - 1);
    return total;
}

void Instruction::decodeInstruction(string binInst)
{
    opcode = posBinaryToInt(binInst.substr(0,6));
    baseReg = posBinaryToInt(binInst.substr(6,5));
    offset = posBinaryToInt(binInst.substr(16,16));
}
