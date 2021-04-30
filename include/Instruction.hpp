#pragma once
#include <string>
#define LW 35   //load word opcode
#define SW 43   //store word opcode

using namespace std;

class Instruction
{
public:
    string binaryInst; //instruction in binary
    int opcode;        //instruction opcode
    int baseReg;       //instruction base register
    int offset;        //memory offset
    int targetReg;     //target register (rt)
    int byteAddress;   //byte address (rs + offset)
    int wordAddress;   //word address (byte address / 4)

    bool hit; //result of access

    int posBinaryToInt(string);     //convert pos binary to an int
    void decodeInstruction(string); //decode 32-bit instruction
    void print();                   //print instruction
    void printBinary();             //print instruction in binary
    void printCacheResult();        //print whether cache read/write hit/miss
};