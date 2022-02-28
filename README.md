# MIPS Memory Hierarchy Simulation
A program that emulates a MIPS CPU reading binary instructions and performing load-word and store-word operations into an L1 cache and main memory module.

The cache used is a two-way set-associative utilizing least recently used write back policy.

![image](https://user-images.githubusercontent.com/46979583/121729240-395d2480-cabc-11eb-8383-f99a8d272a40.png)

## Program Description

This program emulates the manipulation of Cache, Register File, and Memory by instructions executed by a
MIPS CPU. The output displays the instructions, in order, that were executed by the CPU, as well as the contents
of the Register File, Cache, and Main Memory.

## Methodologies/Technologies/Algorithm/Schema/Domain

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

The cache is then checked for a hit or miss. The result is updated in the corresponding element of the instruction array

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

## Inputs and Outputs
Input: 32-bit instructions are received from the objectCode.txt file in resources/

Output: The contents of the Instructions, Registers, Cache, and Main Memory after the execution of all of the
        instructions are displayed.
        
## Compile/Runtime Environment
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
