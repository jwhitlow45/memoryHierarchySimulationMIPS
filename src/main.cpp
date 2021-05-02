#include <iostream>
#include <string>
#include <fstream>

//object header files
#include "Instruction.hpp"
#include "Block.hpp"
#include "Cache.hpp"
#include "Memory.hpp"
#include "RegFile.hpp"

#define NUM_INST 24 //number of instructions in the object code file
#define LW 35       //load word opcode
#define SW 43       //store word opcode

using namespace std;

string twosComp(string); //returns the twos complement of a binary number
void bitPrint(int, int); //prints out an integer in binary

int main()
{
    //Instantiate Cache, Main Memory, and Register File
    RegFile RegisterFile;
    Cache CacheMemory;
    Memory MainMemory;

    //create instruction array to store instructions
    Instruction myInstructions[NUM_INST];

    //create input file stream for reading obj code file
    ifstream FILE;
    FILE.open("resources/objectCode.txt");
    string line;

    //read object code into instruction array
    int i = 0;
    while (FILE >> line)
    {
        myInstructions[i].decodeInstruction(line);
        i++;
    }
    //close file
    FILE.close();

    for (size_t i = 0; i < NUM_INST; i++)
    {
        //set set and tag for operation
        int set = myInstructions[i].wordAddress % 8;
        int tag = myInstructions[i].wordAddress / 8;
        int blockNum; //set block to know which block contains data

        //check cache for hit or miss
        if (CacheMemory.sets[set][0].valid &&
            CacheMemory.sets[set][0].tag == tag)
        { //cache hit
            blockNum = 0;
            myInstructions[i].hit = true;
        }
        else if (CacheMemory.sets[set][1].valid &&
                 CacheMemory.sets[set][1].tag == tag)
        { //cache hit
            blockNum = 1;
            myInstructions[i].hit = true;
        }
        else //cache miss
            myInstructions[i].hit = false;

        if (myInstructions[i].opcode == LW) //perform load word operation
        {
            if (myInstructions[i].hit) //read hit case (store data from cache to register)
            {
                //store cache data in register
                RegisterFile.setReg(myInstructions[i].targetReg,
                                    CacheMemory.sets[set][blockNum].data);
                CacheMemory.setBlockValid(set, blockNum);
                CacheMemory.setBlockHistory(set, blockNum);
            }
            else //read miss case
            {
                //select victim block
                int victimBlock = CacheMemory.getVictimBlock(set);
                //update memory if victim block is valid
                if (CacheMemory.sets[set][victimBlock].valid == 1)
                    MainMemory.memory[(tag * 8 + set)] =
                        CacheMemory.sets[set][victimBlock].data;
                //back up victim block to memory
                CacheMemory.sets[set][victimBlock].data =
                    MainMemory.memory[myInstructions[i].wordAddress];
                //set block history to 1, set valid bit to 1, and update tag
                CacheMemory.setBlockHistory(set, victimBlock);
                CacheMemory.setBlockValid(set, victimBlock);
                CacheMemory.sets[set][victimBlock].tag = tag;
                // store cache data in register file
                RegisterFile.setReg(myInstructions->targetReg,
                                    CacheMemory.sets[set][victimBlock].data);
            }
        }
        else if (myInstructions[i].opcode == SW) //perform store word operation
        {
            if (myInstructions[i].hit) //write hit case
            {
                //store register data into cache
                CacheMemory.sets[set][blockNum].data =
                    RegisterFile.getReg(myInstructions[i].targetReg);
                //write block history and set valid bit to 1
                CacheMemory.setBlockValid(set, blockNum);
                CacheMemory.setBlockHistory(set, blockNum);
            }
            else //write miss case
            {
                //store register data into main memory
                MainMemory.memory[myInstructions[i].wordAddress] =
                    RegisterFile.getReg(myInstructions[i].targetReg);
            }
        }
    }

    //print out instructions
    cout << "\t\t\tInstructions\n";
    cout << "Binary\t\t\t\t\tResult\n";
    for (size_t i = 0; i < NUM_INST; i++)
    {
        cout << myInstructions[i].binaryInst << '\t';
        myInstructions[i].printCacheResult();
        cout << endl;
    }
    cout << endl;

    //print register file contents
    cout << "Register File\n";
    for (size_t i = 0; i < 8; i++)
    {
        cout << 's' << i << '\t';
        bitPrint(RegisterFile.getReg(i + 16), 32);
        cout << endl;
    }

    //print cache contents
    cout << "\n\t\t\tCache\n";
    cout << "block0\t\t\t\t\t\t\t\tblock1\n";
    cout << "set\tvalid\ttag\tdata\t\t\t\t\tvalid\ttag\tdata\n";
    for (size_t i = 0; i < 8; i++)
    {
        cout << i << '\t';
        for (size_t j = 0; j < 2; j++)
        {
            cout << CacheMemory.sets[i][j].valid << '\t';
            if (CacheMemory.sets[i][j].valid)
                bitPrint(CacheMemory.sets[i][j].tag, 4);
            else
                cout << "0000";
            cout << '\t';
            bitPrint(CacheMemory.sets[i][j].data, 32);
            cout << '\t';
        }
        cout << endl;
    }

    //print memory contents
    cout << "\n\t\t\tMain Memory\n";
    cout << "address\t\tdata\t\t\t\tAddress\t\tData\n";
    for (size_t i = 0; i < 64; i++)
    {
        cout << i << '\t';
        bitPrint(MainMemory.memory[i], 32);
        cout << '\t' << i + 64 << '\t';
        bitPrint(MainMemory.memory[i + 64], 32);
        cout << endl;
    }

    return 0;
}

//prints out a given int in binary
void bitPrint(int integer, int bits)
{
    string output = "";

    if (integer == 0) //if int is 0 print zero string of size bits
    {
        for (size_t i = 0; i < bits; i++)
            output += '0';
        cout << output;
        return;
    }

    while (integer != 0) //convert integer to binary
    {
        int rem = abs(integer % 2);
        integer = integer / 2;
        output = char(rem + '0') + output;
    }

    while (output.size() < bits) //pad bits to make bniary correct size
        output = '0' + output;

    cout << output;
}