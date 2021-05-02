#include <iostream>
#include "Memory.hpp"

using namespace std;

void Memory::printMemory()
{
    for (size_t i = 0; i < MEMSIZE; i++)
    {
        cout << memory[i] << '\t';
        if (i % 16 == 0)
            cout << endl;
    }
}