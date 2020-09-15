#include <iostream>
#include <fstream>
#include <cstdint>
#include "Memory.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"
#include "Loader.h"

int main(int argc, char * argv[])
{

   //codes and memory
   RegisterFile * rf = RegisterFile::getInstance();
   ConditionCodes * cc = ConditionCodes::getInstance();
   Memory * mem = Memory::getInstance();

   Loader load(argc, argv);
   if (!load.isLoaded())
      std::cout << "Load error.\nUsage: lab6 <file.yo>\n\n";

   if (rf != NULL) rf->dump();
   if (cc != NULL) cc->dump();
   if (mem != NULL) mem->dump();
}
