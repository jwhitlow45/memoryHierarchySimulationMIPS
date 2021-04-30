#pragma once
#include <string>

using namespace std;

class Instruction
{
public:
    int opcode;      //instruction opcode
    int baseReg;     //instruction base register
    int offset;      //memory offset
    int targetReg;   //target register (rt)
    int byteAddress; //byte address (rs + offset)
    int wordAddress; //word address (byte address * 4)

    int posBinaryToInt(string);
    void decodeInstruction(string);
};