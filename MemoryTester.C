#include <iostream>
#include <assert.h>
#include "Memory.h"
#include "MemoryTester.h"

/** 
 * MemoryTester constructor
 * Initializes the Memory instance 
 *
*/
MemoryTester::MemoryTester()
{
   this->mem = Memory::getInstance();
}

/** 
 * doMemoryTests
 * Calls each individual method to perform tests of the Memory methods
 *
*/
void MemoryTester::doMemoryTests()
{
   assert(this->mem != NULL);
   doClearMemoryTests();
   doPutGetByteTests();
   doPutGetLongTests();
   doByteErrorTests();
   doLongErrorTests();
   std::cout << "All Memory tests passed.\n"; 
}

/**
 * doClearMemoryTests
 * Makes sure that the memory has been initialized to 0
*/
void MemoryTester::doClearMemoryTests()
{
   int32_t addr;
   bool error;
   uint8_t value;
   for (addr = 0; addr < MEMSIZE; addr++)
   {
      value = this->mem->getByte(addr, error);
      //byte retrieved should be 0
      assert(value == 0);
      //address is valid so error should be false
      assert(error == false);
   }
}

/** 
 * doPutGetByteTests
 * Perform tests on Memory::putByte and Memory::getByte
 *
*/
void MemoryTester::doPutGetByteTests()
{
   int32_t addr;
   bool error;
   uint8_t svalue = 0x88;
   uint8_t lvalue;
   for (addr = 0; addr < MEMSIZE; addr++)
   {
      this->mem->putByte(svalue, addr, error);
      //address is valid so error should be false
      assert(error == false);
      lvalue = this->mem->getByte(addr, error);
      //byte retrieved should be the same as that stored
      assert(lvalue == svalue);
      //address is valid so error should be false
      assert(error == false);
   }
}

/** 
 * doPutGetLongTests
 * Perform tests on Memory::putLong and Memory::getLong
 *
*/
void MemoryTester::doPutGetLongTests()
{
   int32_t addr;
   bool error;
   uint64_t svalue = 0x1122334455667788;
   uint64_t lvalue;
   for (addr = 0; addr < MEMSIZE; addr+=8)
   {
      this->mem->putLong(svalue, addr, error);
      //address is valid so error should be false
      assert(error == false);
      lvalue = this->mem->getLong(addr, error);
      //long retrieved should be the same as that stored
      //printf("getLong lvalue = %x\n", lvalue);
      //printf("putLong svalue = %x\n", svalue);
      assert(lvalue == svalue);
      //address is valid so error should be false
      assert(error == false);
   }
}

/** 
 * doByteErrorTests
 * Perform error tests on Memory::getByte and Memory::putByte
 *
*/
void MemoryTester::doByteErrorTests()
{
   int32_t addr;
   bool error;
   uint8_t svalue = 0x88;
   uint8_t lvalue;
   //test on a random range of addresses
   for (addr = -0x2000; addr < 0x2000; addr++)
   {
      if (addr < 0 || addr >= MEMSIZE)
      {
         this->mem->putByte(svalue, addr, error);
         //address is invalid so error should be true
         assert(error == true);
         lvalue = this->mem->getByte(addr, error);
         //address is invalid so getByte should return 0
         assert(lvalue == 0);
         //address is invalid so error should be true
         assert(error == true);
      }
   }
}

/** 
 * doLongErrorTests
 * Perform error tests on Memory::getLong and Memory::putLong
 *
*/
void MemoryTester::doLongErrorTests()
{
   int32_t addr;
   bool error;
   uint64_t svalue = 0x1122334455667788;
   uint64_t lvalue;
   for (addr = -0x2000; addr < 0x2000; addr++)
   {
      if (addr < 0 || addr >= MEMSIZE || addr % 8 != 0)
      {
         //address is invalid so error should be true
         this->mem->putLong(svalue, addr, error);
         assert(error == true);
         lvalue = this->mem->getLong(addr, error);
         //address is invalid so 0 returned by getLong
         assert(lvalue == 0);
         //address is invalid so error should be true
         assert(error == true);
      }
   }
}

