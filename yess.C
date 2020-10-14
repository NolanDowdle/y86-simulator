/* 
 * Driver for the yess simulator
 * Usage: yess <file>.yo [-D]
 *
 * <file>.yo contains assembled y86-64 code.
 * If the -D option is provided then debug is set to 1.
 * The -D option can be used to turn on and turn off debugging print
 * statements.
*/

#include <iostream>
#include <fstream>
#include <string.h>
#include "Debug.h"
#include "Memory.h"
#include "Loader.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"
#include "PipeReg.h"
#include "Stage.h"
#include "Simulate.h"

int debug = 0;

int main(int argc, char * argv[])
{
   //check to see if the -D option was provided 
   if (argc >= 3 && (strcmp(argv[2], "-D") == 0)) debug = 1;

   Memory * mem = Memory::getInstance();
   Loader load(argc, argv);
   if (!load.isLoaded())
   {
      std::cout << "Load error.\nUsage: yess <file.yo>\n";
      if (mem != NULL) mem->dump();
      return 0;
   }
  
   Simulate simulate;

   //added lines 42 through 46 in order to clear all registers before running the simulation
   RegisterFile * regInstance = RegisterFile::getInstance();
    bool error;
    for (unsigned int i = 0; i < 15; i++) {
       regInstance->writeRegister(0, i, error);
    }


   simulate.run(); 
   
   return 0;
}

