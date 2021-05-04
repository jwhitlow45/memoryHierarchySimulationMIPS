/*
GLOBAL DOCUMENTATION

--Program Description--
    This program emulates the manipulation of Cache, Register File, and Memory by instructions executed by a
    MIPS CPU. The output displays the instructions, in order, that were executed by the CPU, as well as the contents
    of the Register File, Cache, and Main Memory.

--Methodologies/Technologies/Algorithm/Schema/Domain--
    To build this program I broke each major part down and made a class for each. By doing so the code is much more
    organized and will be easier to read in the future. It also allows for the scoping of methods, making it clear
    which methods are acting on which objects.

    The file structure of the program is as follows:

        root/
            L>include/
                Block.hpp
                Cache.hpp
                Instruction.hpp
                Memory.hpp
                RegFile.hpp
            L>src/
                Cache.cpp
                Instruction.cpp
                main.cpp
                RegFile.cpp
            L>resources/
                L>objectCode.txt

    The root folder contains 3 folders; include, resources, and src. The include folder contains header files for
    each of the objects used in the project, with the methods for these header files defined in the src folder. The
    resources folder contains the objectCode.txt file, the location where all of the instructions will be fetched from.
    Looking at these header files we can see the component parts of the objects.
    
        A Block object was created allowing for easy creation of multiple blocks to make up a cache. In the
        Cache header we can see the declaration of a 2d array of blocks of size 8x2. This is created a cache with 8
        sets with 2 blocks per set (creating a 2-way set-associative cache). The Cache object also contains methods to
        manipulate the history of a block, set a block as valid, or get the victim block for read miss cases.

        An Instruction object was also created so that the input file could be read all at once. This object contains
        variables to store the binary instruction, opcode, base register, offset, raget register, byte address, and
        word address of an instruction. It also stores whether or not an instruction lead to a hit or a miss. Methods
        were also included which allowed for converting binary to an integer, decoding insturctions, printing contents
        of the object, printing the binary instruction, and printing the cache result (read/write hit/miss).

        A Memory object was created to allow it to be clear whenever any memory accesses occured. This object contains
        a single array that is of size 128, representing memory that is 128 words in size.

        The RegFile object contains an array of size 8, representing each of the 8 registers that are going to be used
        throughout the emulation. It also contains a zero constant representing the zero register. Two methods are also
        defined which are getReg and setReg. Get reg returns the contents of a register from its rt representation. In
        this case the registers can be found in the reg array at index i - 16, where i is the rt integer representation
        of the number. Conversely, setReg sets a given register based on its rt integer representation a a value to be
        stored.

    The program starts by instantiating 3 objects, the RegisterFile, CacheMemory, and MainMemory. These are the 3
    component parts that make up the program. We then initiazlie all the variables in these objects to avoid any
    segment faults when displaying the registers later in the program. An Instruction array is created to contain
    the 24 instructions in the objectCode.txt.

    We then create an ifstream allowing for the input of data from the objectCode.txt file. Each instruction is decoded
    into an instruction in the myInstruction array one by one, in order. The ifstream is then closed.

    For each instruction in the instruction array, the following algorithm is ran.

        The set and tag fields are calculated and a blockNum to track the blockNum we are using.

        The cache is then checked for a hit or miss. The result is updated in the corresponding element of the
        instruction array

        The opcode of the instruction is checked to determine which operation to perform (LW or SW)

            if LW is performed
                if the instruction was a hit then we perform a read hit
                if the instruction was a miss then we perform a read miss

            else if SW is performed
                if the instruction was a hit then we perform a write hit
                if the instruction was a miss then we perform a write miss

    After the algorithm is ran for each instruction the resulting contents of each components are displayed. Since the
    values are stored as integers we use an integer to binary converter called bitPrint to show the contents as binary.
    Given an integer and a number of bits the instruction displays the integer in binary bit extended to match the
    number of bits specified.

--Inputs and Outputs--
    Input: 32-bit instructions are received from the objectCode.txt file in resources/
    Output: The contents of the Instructions, Registers, Cache, and Main Memory after the execution of all of the
            instructions are displayed.

--Compile/Runtime Environment--
    This program was built for C++17 using the G++ compiler and GDB for debugging on Pop!OS 20.10 (Ubuntu). The code
    can be compiled using the following configuration.

            CXX       := g++
            CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

            BIN     := bin
            SRC     := src
            INCLUDE := include
            LIB     := lib

            LIBRARIES   :=
            EXECUTABLE  := main

    src/ must contain all .cpp files, include/ must contain all .hpp files, and resources/ must contain objectCode.txt 

*/

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

void bitPrint(int, int); //prints out an integer in binary

int main()
{
    //Instantiate Cache, Main Memory, and Register File
    RegFile RegisterFile;
    Cache CacheMemory;
    Memory MainMemory;

    //initialize cache and memory
    for (size_t i = 0; i < 8; i++)
    {
        RegisterFile.reg[i] = 0;
        for (size_t j = 0; j < 2; j++)
        {
            CacheMemory.sets[i][j].valid = 0;
            CacheMemory.sets[i][j].tag = 0;
            CacheMemory.sets[i][j].data = 0;
        }
    }

    for (size_t i = 0; i < 128; i++)
        MainMemory.memory[i] = i + 5;

    //create instruction array to store instructions
    Instruction* myInstructions = new Instruction[NUM_INST];

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
                RegisterFile.setReg(myInstructions[i].targetReg,
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

    delete[] myInstructions;    //clean up dynamic memory

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

    while (output.size() < bits) //pad bits to make binary correct size
        output = '0' + output;

    cout << output;
}