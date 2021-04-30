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
    //Instantiate Cache, Main Memory, and REgister File
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
            if (myInstructions[i].hit) //read hit case
            { 
            }
            else //read miss case
            {
            }
        }
        else if (myInstructions[i].opcode == SW) //perform store word operation
        {
            if (myInstructions[i].hit) //store hit case
            {
            }
            else //store miss case
            {
            }
        }
    }

    return 0;
}

string twosComp(string binary)
{
    //flip all binary bits
    for (size_t i = 0; i < binary.size(); i++)
    {
        if (binary[i] == '0')
            binary[i] = '1';
        else
            binary[i] = '0';
    }

    int sum = 1; //added 1
    int i = binary.size() - 1;

    //add 1
    while (i > -1)
    {
        sum = binary[i] - '0' + sum;
        if (sum == 1)
        {
            binary[i] = '1';
            return binary;
        }

        //otherwise sum is 2
        binary[i] = '0';
        sum = 1;
        i--;
    }
}

void bitPrint(int integer, int bits)
{
    string output = "";
    bool negative = false;

    if (integer < 0) //if value is negative set negative flag
        negative = true;
    else if (integer == 0) //if int is 0 print zero string of size bits
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

    if (negative) //print twos complement if value is negative
        cout << twosComp(output);
    else
        cout << output;
}