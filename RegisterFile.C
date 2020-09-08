#include <iostream>
#include <iomanip>
#include "RegisterFile.h"
#include "Tools.h"

//regInstance will be initialized to the single RegisterFile
//object that is created
RegisterFile * RegisterFile::regInstance = NULL;

/**
 * RegisterFile constructor
 * initialize the contents of the reg array to 0
 */
RegisterFile::RegisterFile()
{
    reg[15] = { };  
}

/**
 * getInstance
 * if regInstance is NULL then creates a Register object
 * and sets regInstance to point it; returns regInstance
 *
 * @return regInstance, the pointer to the single RegisterFile
 *         instance
 */
RegisterFile * RegisterFile::getInstance()
{
   if(regInstance == NULL) {
       regInstance = new RegisterFile;
   }
   return regInstance;
}

/**
 * readRegister
 * returns a register value from the reg array.
 * if regNumber is a valid register number then 
 * sets error to false and returns
 * the register value; otherwise sets error to
 * true and returns 0 
 *
 * @param register number
 * @returns reg[regNumber] if regNumber is valid, otherwise 0
 * @returns sets error to false if regNumber is valid, otherwise true
*/
uint64_t RegisterFile::readRegister(int32_t regNumber, bool & error)
{
   if (regNumber >=0 && regNumber <= 14) {
       error = false;
       return reg[regNumber];
   }
   error = true;
   return 0;
}

/**
 * writeRegister
 * sets a register to a specified value if the regNumber is
 * within range and sets error to false; otherwise, sets error to true
 *
 * @param value to be stored in reg[regNumber]
 * @param number of register to be modified (index into reg array)
 * @returns sets error to false if regNumber is valid and true otherwise
 */
void RegisterFile::writeRegister(uint64_t value, int32_t regNumber, 
                                 bool & error)
{
    if (regNumber >= 0 && regNumber <= 14) {
        reg[regNumber] = value;
        error = false;
    }
    else {
        error = true;
    }
}

/**
 * dump
 * output the contents of the reg array
 */
void RegisterFile::dump()
{
   std::string rnames[15] = {"%rax: ", "%rcx: ", "%rdx: ",  "%rbx: ",
                             "%rsp: ", "%rbp: ", "%rsi: ",  "%rdi: ", 
                             "% r8: ", "% r9: ", "%r10: ",  "%r11: ",
                             "%r12: ", "%r13: ", "%r14: "};  
   for (int32_t i = 0; i < REGSIZE; i+=4)
   {
      for (int32_t j = 0; j < 3; j++)
         std::cout << rnames[i + j] << std::hex << std::setw(16) 
                   << std::setfill('0') << reg[i + j] << ' ';
      if (i + 3 < REGSIZE) 
         std::cout << rnames[i + 3] << std::hex << std::setw(16) 
                   << std::setfill('0') << reg[i + 3] << std::endl;
      else
         std::cout << std::endl;
   }
}
